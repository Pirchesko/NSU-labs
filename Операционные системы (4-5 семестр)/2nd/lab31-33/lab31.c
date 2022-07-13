#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h> 
#include <string.h>
#include <netdb.h>
#include <signal.h> 
#include <fcntl.h>

#include "http_parser.h"

#define PORT 55555
#define MAX_CONNECTIONS 50
#define BUF 1024

#define ERROR_505 "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n"
#define ERROR_504 "HTTP/1.1 504 Gateway Timeout\r\n\r\n"
#define ERROR_405 "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
#define ERROR_403 "HTTP/1.1 403 Forbidden\r\n\r\n"
#define ERROR_400 "HTTP/1.1 400 Bad request\r\n\r\n"

struct pollfd polls[MAX_CONNECTIONS * 2 + 1];
int connections = 0; //количество соединений (склиент-сервер и сервер-клиент)
int signalExit = 1; 
int recv_errors = 0; //debug: остсеживание получения пустых recv() с клиентов
int clients_cache = 0; //счётчик клиентов, получающие данных из кэша

// -------------------------------------- HTTP parser -----------------------------------------------

http_parser_settings p_sett_request;
http_parser request_parser;
http_parser_settings p_sett_response;
http_parser response_parser;

struct request_info {
    char* method;
    char* url;
    char* hostname;
    int port;
};

struct response_info {
    char* status_code;
};

int on_headers_complete(http_parser* parser)
{
    struct request_info* info = (struct request_info*)parser->data;
    info->method = (char*)calloc(strlen(http_method_str(parser->method)) + 1, sizeof(char));
    if (info->method == NULL)
    {
        return -1;
    }
    strcpy(info->method, http_method_str(parser->method));
    return 0;
}

char* get_line(const char* str, int pos, int length)
{
    char* line = (char*)calloc((length + 1), sizeof(char));
    if (line == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < length; i++)
    {
        line[i] = str[i + pos];
    }
    line[length] = '\0';
    return line;
}

int on_url(http_parser* parser, const char* at, size_t length)
{
    static struct http_parser_url parser_url;
    http_parser_url_init(&parser_url);
    struct request_info* info = (struct request_info*)parser->data;
    info->url = (char*)calloc((length + 1), sizeof(char));
    if (info->url == NULL)
    {
        return -1;
    }
    memcpy(info->url, at, length);
    info->url[length] = '\0';
    //printf("Header field: %s\n", info->url);
    int result = http_parser_parse_url(info->url, strlen(info->url), 0, &parser_url);
    if (result != 0)
    {
        info->hostname = NULL;
        //printf("Parse error : %s\n", info->url);
        return result;
    }
    else
    {
        info->hostname = get_line(info->url, parser_url.field_data[1].off, parser_url.field_data[1].len);
        char* port = get_line(info->url, parser_url.field_data[2].off, parser_url.field_data[2].len);
        if (port == NULL)
        {
            return -1;
        }
        if (strcmp(port, "") == 0)
        {
            info->port = 80;
        }
        else
        {
            info->port = atoi(port);
        }
        free(port);
    }
    return 0;
}

int on_status(http_parser* parser, const char* at, size_t length)
{
    struct response_info* info = (struct response_info*)parser->data;
    //info->status_code = (char*)calloc(strlen(http_status_str((http_status)parser->status_code) + 1), sizeof(char));
    info->status_code = (char*)calloc((length + 1), sizeof(char));
    if (info->status_code == NULL)
    {
        return -1;
    }
    //strcpy(info->status_code, http_method_str((http_method)parser->method));
    memcpy(info->status_code, at, length);
    info->status_code[length] = '\0';
    //printf("Response_status: %s\n", info->status_code);
    return 0;
}

void init_parsers_response()
{
    http_parser_init(&response_parser, HTTP_RESPONSE);
    p_sett_response.on_status = on_status;
}

void init_parsers_request()
{
    http_parser_init(&request_parser, HTTP_REQUEST);
    p_sett_request.on_headers_complete = on_headers_complete;
    p_sett_request.on_url = on_url;
}

//---------------------------------------------------------------------------------------------------

//-------------------------------------------- Cache ------------------------------------------------

typedef struct Cache {
    char* url;
    char* buf;
    int size_buf;
    int status; //-1 - произошла ошибка в кэше при записи, т.е. не следует больше его читать, а следует удалить кэш-запись; 0 - кэш-запись в порядке; 1 - кэш-запись записывается (запись кэша и отправка клиенту при записи); 2 - кэш-запись записывается, но клиенту не отправляются полученный данные (только запись кэша)
    struct Cache* next;
    //pthread_mutex_t mutex;
} Cache;

Cache* cache_list;
Cache* cache_fast[MAX_CONNECTIONS * 2] = { 0 }; //Быстрый доступ в кэш для каждого polls[i] (указатель на указатель в кэш)
int status_cache[MAX_CONNECTIONS * 2] = { 0 }; 
// [MAX_CONNECTIONS; MAX_CONNECTIONS*2]: 0 - ничего не происходило; -200 - работаем без кэша; -1 - ошибка при записи кэша; >0 размер записанного кэша на данный момент
// [0; MAX_CONNECTIONS]: 0 - ничего не происходило; -200 - работаем без кэша (тут не пригодилось); -1 - какая-то ошибка; >0 размер считанного кэша на данный момент
char* cache_URLs[MAX_CONNECTIONS] = { 0 }; //Быстрый способ добраться до url (выделяется память)
int size_cache_list = 0; //Счётчик кэш-записей в кэше

Cache* init_cache_head()
{
    Cache* head = (struct Cache*)calloc(1, sizeof(Cache));
    head->url = NULL;
    head->buf = NULL;
    head->next = NULL;
    //pthread_mutex_init(&(head->mutex), NULL);
    return head;
}

//Очистка кэша начиная с конкретного указателя
void clear_cache_from(Cache* head)
{
    if (head == NULL)
    {
        return;
    }
    Cache* tmpCache;
    int size_cache_free = 0;
    for (Cache* iter = head; iter; iter = tmpCache)
    {
        //pthread_mutex_destroy(&(iter->mutex));
        tmpCache = iter->next;
        size_cache_free += iter->size_buf;
        free(iter->buf);
        free(iter->url);
        free(iter);
        size_cache_list--;
    }
    printf("clear = %d KB; ", size_cache_free / 1024);
}

//Отчитываем с какой записи будем стриать кэш
void clear_part_cache(Cache* head, int count_field_to_free)
{
    if (head == NULL)
    {
        return;
    }
    Cache* tmpCache = head->next;
    int list_position = 0;
    int size_all_cache = 0;
    while ((tmpCache != NULL) && (list_position < size_cache_list - count_field_to_free))
    {
        size_all_cache += tmpCache->size_buf;
        tmpCache = tmpCache->next;
        list_position++;
    }
    Cache* tmpCache_ = tmpCache;
    tmpCache_ = tmpCache->next;
    tmpCache->next = NULL;
    size_all_cache += tmpCache->size_buf;
    printf("size cache = %d KB; ", size_all_cache / 1024);
    clear_cache_from(tmpCache_);
}

//Посик URL в кэше
Cache* check_url(Cache* head, char* url)
{
    if (head == NULL)
    {
        return NULL;
    }
    Cache* tmpCache = head->next;
    while (tmpCache != NULL)
    {
        if (strcmp(tmpCache->url, url) == 0)
        {
            return tmpCache;
        }
        tmpCache = tmpCache->next;
    }
    return NULL;
}

//Создание кэш записи
Cache* create_cache(Cache* head, char* url, int size) //push in head cache_list struct
{
    if (head == NULL)
    {
        return NULL;
    }
    Cache* cache = (struct Cache*)calloc(1, sizeof(Cache));
    if (cache == NULL)
    {
        free(cache);
        perror("Error creating cach");
        return NULL;
    }
    //pthread_mutex_init(&(node->mutex), NULL);
    cache->next = NULL;
    cache->url = (char*)calloc(size + 1, sizeof(char));
    if (strcpy(cache->url, url) == NULL)
    {
        free(cache->url);
        free(cache);
        perror("Error creating cach (url calloc)");
        return NULL;
    }
    cache->buf = NULL;
    //lockMutex(&(head->mutex));
    cache->next = head->next;
    head->next = cache;
    size_cache_list++;
    //unlockMutex(&(head->mutex));
    return cache;
}

//Запись информации в кэш
int write_cache(Cache* cache, int id, char* buf, int size)
{
    if (cache->buf == NULL)
    {
        cache->buf = (char*)calloc(size, sizeof(char));
        if (cache->buf == NULL)
        {
            free(cache->buf);
            free(cache->url);
            free(cache);
            perror("Error in writing cache - calloc()"); //TODO: что делать если ошибка?????
            status_cache[id] = -1;
            return -1;
        }
        status_cache[id] = size;
        cache->size_buf = status_cache[id];
        if (memcpy(cache->buf, buf, size) == NULL)
        {
            free(cache->buf);
            free(cache->url);
            free(cache);
            perror("Error in writing cache"); //TODO: что делать если ошибка?????
            status_cache[id] = -1;
            return -1;
        }
    }
    else
    {
        cache->buf = (char*)realloc(cache->buf, status_cache[id] + size);
        if (cache->buf == NULL)
        {
            free(cache->buf);
            free(cache->url);
            free(cache);
            perror("Error in writing cache - realloc()"); //TODO: что делать если ошибка?????
            //status_cache[id] = -1;
            return -1;
        }
        status_cache[id] = status_cache[id] + size;
        cache->size_buf = status_cache[id];
        if (memcpy(cache->buf + status_cache[id] - size, buf, size) == NULL)
        {
            free(cache->buf);
            free(cache->url);
            free(cache);
            perror("Error in writing cache"); //TODO: что делать если ошибка?????
            //status_cache[id] = -1;
            return -1;
        }
    }
    //printf("[CACHE | DEBUG] Size buf = %d\n", strlen(cache->buf));
    return status_cache[id]; //Что возращать?
}

//Чтенение информации из кэша
int read_cache(Cache* cache, int id, char* buf)
{
    if (cache->buf == NULL)
    {
        return -1;
    }
    else
    {
        int mem_size = 0;
        int read_size = cache->size_buf - status_cache[id];
        if (read_size < BUF)
        {
            mem_size = read_size;
        }
        else
        {
            mem_size = BUF;
        }
        if (memcpy(buf, cache->buf + status_cache[id], mem_size) == NULL)
        {
            perror("Error in reading cache"); //TODO: что делать если ошибка?????
            return -1; 
        }
        status_cache[id] += mem_size;
        return mem_size;
    }
}

//---------------------------------------------------------------------------------------------------

//кэш докачивать всегда до конца

//Завершение соединения со сторон сервера и клиента
void finish_connection(struct pollfd* polls, int id)
{
    close(polls[id].fd);
    close(polls[id - MAX_CONNECTIONS].fd);
    polls[id].fd = -1;
    polls[id - MAX_CONNECTIONS].fd = -1;
    //status[id - MAX_CONNECTIONS] = 0;
    status_cache[id] = 0;
    status_cache[MAX_CONNECTIONS - id] = 0;
    free(cache_URLs[id - MAX_CONNECTIONS]);
    cache_URLs[id - MAX_CONNECTIONS] = NULL;
    cache_fast[id] = NULL;
    connections--;
    printf("Connection dropped: id = %d; id = %d; connections = %d (after drop)\n", id, id - MAX_CONNECTIONS, connections);
}

//Подключение к серверу (url - сайтам)
int open_connection(struct request_info* info)
{
    int socket_fd = -1;
    int op = fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    struct addrinfo* hints, * res, * rp;
    char str_port[11];
    int result;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; //????
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    snprintf(str_port, 11, "%d", info->port);

    if ((result = getaddrinfo(info->hostname, str_port, hints, &res)) != 0)
    {
        free(hints);
        return -1;
    }
    
    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
        socket_fd = socket(rp->ai_family, rp->ai_socktype | SOCK_NONBLOCK, rp->ai_protocol);
        int mask = fcntl(socket_fd, F_SETFD, O_NONBLOCK);
        if (socket_fd == -1)
        {
            continue;
        }
        //(res->ai_addr)->sin_port = htons(80);

        connect(socket_fd, rp->ai_addr, rp->ai_addrlen);

        struct pollfd poll_connect;
        poll_connect.fd = socket_fd;
        poll_connect.events = POLLOUT;
        int p = poll((struct pollfd*)&poll_connect, 1, 5000);
        if (p == 1)
        {
            mask = mask & (~O_NONBLOCK);
            fcntl(socket_fd, F_SETFD, mask);
            break;
        }
        close(socket_fd);
    }

    free(hints);
    freeaddrinfo(res);
    if (rp == NULL)
    {
        return -1;
    }
    return socket_fd;
}

int findFreePoll(struct pollfd* polls)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (polls[i].fd == -1)
        {
            return i;
        }
    }
    return -1;
}

//Возращает текущий IP адрес (не используется - для debug)
void getServerIP(char addressBuffer[INET_ADDRSTRLEN])
{
    struct ifaddrs* ifAddrStruct = NULL;
    struct ifaddrs* ifa = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        if ((ifa->ifa_addr->sa_family == AF_INET) && (ifa->ifa_name[0] == 'e') && (ifa->ifa_name[3] == '0'))
        {
            tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("Using for server this address (local IP): %s %s\n", ifa->ifa_name, addressBuffer);
        }
    }

    if (ifAddrStruct != NULL)
    {
        freeifaddrs(ifAddrStruct);
    }
}

//Завершение соединение только со стороны клиента с конкретной ошибкой или нет или без неё
void bad_errors(struct pollfd* polls, int id, int error)
{
    char* text_error;
    if (error != -1)
    {
        switch (error)
        {
        case 505: text_error = ERROR_505; break;
        case 504: text_error = ERROR_504; break;
        case 405: text_error = ERROR_405; break;
        case 403: text_error = ERROR_403; break;
        case 400: text_error = ERROR_400; break;
        }
        char buf[BUF];
        polls[id].revents = 0;
        int size_buf = strlen(text_error);
        sprintf(buf, "%s", text_error);//! \0
        int size_buf_write = send(polls[id].fd, buf, size_buf, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
        if (size_buf_write != size_buf)
        {
            printf("[WARING] ERROR! Error in_bad_errors\n");
        }
    }
    close(polls[id].fd);
    polls[id].fd = -1;
}

int delete_keep_alive(char* buf, int size)
{
    char* pos = strcasestr(buf, "keep-alive");
    if (pos == NULL)
    {
        return -1;
    }
    char* newpos = &pos[10];
    int size_3 = strlen(newpos);
    char* s = strcpy(pos, "close");
    memmove(s+5, newpos, size_3);
    char* rnrn = strstr(buf, "\r\n\r\n");
    if (rnrn == NULL)
    {
        rnrn = strstr(buf, "\n\n");
        rnrn[2] = '\0';
    }
    else
    {
        rnrn[4] = '\0';
    }
    return strlen(buf);
}

//Обработчик чтения из кэша для клиента
void write_from_cache(struct pollfd* polls, int id, Cache* cache_head)
{
    //char buf[BUF] = { 0 };
    if ((status_cache[id + MAX_CONNECTIONS] != 0) && (status_cache[id] == status_cache[id + MAX_CONNECTIONS])) //мы в состоянии того, что кэш одновременно качается и читается, и мы вынуждены подождать, когда оно в кэше появиться
    {
        return;
    }
    else
    {
        char buf[BUF] = { 0 };
        int read_cache_size = read_cache(cache_fast[id], id, buf);
        if (read_cache_size >= 0)
        {
            if (read_cache_size == 0) //прочитали весь кэш и можем завершать соединение
            {
                if (cache_fast[id]->status > 0) //но это оказался не весь кэш, поэтому ждём (был бы весь, статус = 0 оставили)
                {
                    return;
                }
                printf("[CACHE | C->S | Finish send cache] id = %d; read_cache_size = %d; total_read = %d (%s)\n", id, read_cache_size, status_cache[id], cache_fast[id]->url);
                clients_cache--;
                close(polls[id].fd);
                polls[id].fd = -1;
                status_cache[id] = 0;
                cache_fast[id] = NULL;
                return;
            }
            int size_buf_write = send(polls[id].fd, buf, read_cache_size, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
            if (size_buf_write != size_buf_write)
            {
                printf("[CACHE | C->S | write()] ERROR! Sizes of read cache and send is not equals! (%s)\n", cache_fast[id]->url);
                //завершать соединение???
                clients_cache--;
                bad_errors((struct pollfd*)polls, id, -1);
                return;
            }
        }
        else
        {
            printf("[CACHE | C->S | read() ] ERROR! read_cache() have error! (%s)\n", cache_fast[id]->url);
            //завершать соединение???
            clients_cache--;
            bad_errors((struct pollfd*)polls, id, -1);
            return;
        }
    }
}

//Обработчик соединения клиент-сервер
void fromClient_toServer(struct pollfd* polls, int id, Cache* cache_head)
{
    if ((polls[id].fd == -1) || (status_cache[id] == -200))
    {
        if (status_cache[id] == -200)
        {
            printf("status_cache[id] == -200\n");
        }
        return;
    }
    else if ((polls[id].revents == POLLIN) || status_cache[id] > 0)
    {
        if (status_cache[id] <= 0) 
        {
            polls[id].revents = 0;
        }

        if (status_cache[id] == 0) //Тут мы в первый раз (и возможно последний)
        {
            char buf[BUF] = { 0 };
            int size_buf = recv(polls[id].fd, buf, sizeof(buf), MSG_NOSIGNAL); //read(polls[id].fd, buf, sizeof(buf));
            if (size_buf > 0) //Первый recv() как правило всегда >0
            {
                int size_buf_KA = delete_keep_alive(buf, size_buf);
                if (size_buf_KA == -1) //Не нашли поля keep-alive или ошибка в замене его на close
                {
                    size_buf_KA == size_buf;
                    printf("[C->S | Waring | delete_keep_alive()] Keep-alive is not detected or error!\n");
                }

                http_parser_init(&request_parser, HTTP_REQUEST);
                struct request_info* request_info = (struct request_info*)calloc(1, sizeof(struct request_info));
                if (request_info == NULL) //Ошибка выделения памяти
                {
                    free(request_info);
                    printf("[C->S | Waring | request_info] Error in calloc() for request!\n");
                    bad_errors((struct pollfd*)polls, id, -1);
                    return;
                }

                request_parser.data = request_info;
                size_t parse_size = http_parser_execute(&request_parser, &p_sett_request, buf, (size_t)size_buf_KA);
                if (parse_size != size_buf_KA)
                {
                    printf("[C->S | Waring | http_execute()] HTTPS is not supported! Or maybe parser error. (%s)\n", request_info->url);
                    bad_errors((struct pollfd*)polls, id, 403);
                    return;
                }

                cache_fast[id] = check_url(cache_head, request_info->url); //Проверяем, есть ли в кэше
                if ((cache_fast[id] != NULL) && (cache_fast[id]->status == -1))
                {
                    cache_fast[id] = NULL; //В кэше обнаружилась запись, но она бракованная, делаем вид что записи нет (потом она удалится)
                }
                if (cache_fast[id] == NULL) //В кэше записи нет
                {
                    if ((request_parser.http_major != 1) || (request_parser.http_minor != 0))
                    {
                        printf("[C->S | Waring | version hhtp] Bad version HTTP: %d.%d (%s)\n", request_parser.http_major, request_parser.http_minor, request_info->url);
                        bad_errors((struct pollfd*)polls, id, 505);
                        return;
                    }
                    //if (strcmp(request_info->method, "GET") != 0) //Можно обрабатывать только GET
                    //{
                    //    printf("[WARING] Bad method HTTP: %s\n", request_info->method);
                    //    bad_errors((struct pollfd*)polls, id, 405);
                    //    return;
                    //}

                    int connect_fd = open_connection(request_info); //Подключаемся к серверу
                    if (connect_fd <= 0)
                    {
                        printf("[C->S | Waring | connect()] Error in open connetcion: 504 Tiomeout; (%s)\n", request_info->url);
                        bad_errors((struct pollfd*)polls, id, 504);
                        return;
                    }

                    polls[id + MAX_CONNECTIONS].fd = connect_fd;
                    printf("[C->S] Create connection!\t Client: id = %d; FD = %d; size = %d\t Server: id = %d; FD = %d; (%s)\n", id, polls[id].fd, size_buf, id + MAX_CONNECTIONS, polls[id + MAX_CONNECTIONS].fd, request_info->url);
                    connections++;

                    int size_buf_write = send(polls[id + MAX_CONNECTIONS].fd, buf, size_buf, MSG_NOSIGNAL); //write(polls[id + MAX_CONNECTIONS].fd, buf, size_buf);
                    if (size_buf_write != size_buf)
                    {
                        printf("[C->S | Waring | send()] Error in send from Client to Server request! (%s)\n", request_info->url);
                        bad_errors((struct pollfd*)polls, id, -1);
                        return;
                    }

                    //cache_URLs[id]
                    cache_URLs[id] = (char*)calloc(strlen(request_info->url) + 1, sizeof(char));
                    if (strcpy(cache_URLs[id], request_info->url) == NULL)
                    {
                        free(cache_URLs[id]);
                        printf("[C->S | Waring | send()] Error in remember url for cache (url calloc) (%s)\n", request_info->url);
                        return;
                    }
                }
                else //Нашлась кэш-запись, начинаем работу с кэшем
                {
                    clients_cache++;
                    write_from_cache((struct pollfd*)polls, id, cache_head);
                }
            }
            else //Первый recv() и он меньше нуля => какая-то ошибка
            {
                printf("[C->S | Waring | recv()] (Close connection) Error in recv()=0 from Client to connect to the server!, id = %d; size_buf = %d\n", id, size_buf);
                recv_errors++;
                bad_errors((struct pollfd*)polls, id, -1);
                return;
            }
        }
        else //тут мы не первый раз, а значит рыботаем с кэшом:
        {
            write_from_cache((struct pollfd*)polls, id, cache_head);
        }
    }
}

//Отправка данных от сервера к клиенту
int sender_SC(struct pollfd* polls, int id, char* buf, int size_buf_read)
{
    int size_buf_write = send(polls[id - MAX_CONNECTIONS].fd, buf, size_buf_read, MSG_NOSIGNAL);//write(polls[id - MAX_CONNECTIONS].fd, buf, size_buf_read);
    if (size_buf_write != size_buf_read)
    {
        printf("[S->C | Waring | send] Error in send().\n"/*, cache_fast[id]->url*/);
        //finish_connection((struct pollfd*)polls, id);
        return -1;
    }
    return size_buf_write;
}

//Обработчик севрер-клиент (->)
void fromServer_toClient(struct pollfd* polls, int id, Cache* cache_head)
{
    if ((polls[id].fd == -1) || (polls[id].revents != POLLIN))
    {
        return;
    }
    polls[id].revents = 0;

    char buf[BUF];
    int size_buf_read = recv(polls[id].fd, buf, sizeof(buf), MSG_NOSIGNAL); //read(polls[id].fd, buf, sizeof(buf));
    if (size_buf_read > 0)
    {
        if (status_cache[id] == 0) //Первое вхождение => парсим заголовок ответа и выясняем надо ли кэшировать
        {
            int skip_caching = 0;
            http_parser_init(&request_parser, HTTP_RESPONSE);
            struct response_info* response_info = (struct response_info*)calloc(1, sizeof(struct response_info));
            if (response_info == NULL) //ошибка выделения памяти
            {
                free(response_info);
                printf("[S->C | Waring | response_info] Error in calloc() for status_code. Working without cahce. (%s)\n", cache_fast[id]->url);
                skip_caching = 1;
            }

            request_parser.data = response_info;
            size_t parse_size = http_parser_execute(&request_parser, &p_sett_response, buf, (size_t)size_buf_read);
            if (parse_size != size_buf_read) //Неправильно распарсен ответ
            {
                printf("[S->C | Waring | parser_execute] Parser error. Don't know status_code. Working without cahce. (%s)\n", cache_fast[id]->url);
                skip_caching = 1;
            }

            if ((skip_caching == 1) || (strcmp(response_info->status_code, "OK") != 0)) //кэшируем только 200 OK
            {
                status_cache[id] = -200;
                if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0) //работаем без кэша
                {
                    finish_connection((struct pollfd*)polls, id);
                }
                return;
            }

            //Начинаем кэширование
            if (cache_URLs[id - MAX_CONNECTIONS] != NULL) //Быстрая кэш запись есть => можем добалять в кэш
            {
                cache_fast[id] = create_cache(cache_head, cache_URLs[id - MAX_CONNECTIONS], strlen(cache_URLs[id - MAX_CONNECTIONS]));
                if (cache_fast[id] == NULL) //Кэш-запись не была создана, работаем без кэша
                {
                    printf("[S->C | CACHE | Waring | create_cache()] Error in create cache. Working without cahce. (%s)\n", cache_fast[id]->url);
                    status_cache[id] = -200;
                    if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0)
                    {
                        finish_connection((struct pollfd*)polls, id);
                    }
                    return;
                }
                cache_fast[id]->status = 1;
                printf("[C->S | CACHE | start caching] Caching! id = %d; (%s)\n", id, cache_fast[id]->url);
                int write_cache_size = write_cache(cache_fast[id], id, buf, size_buf_read);
                if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                {
                    printf("[S->C | CACHE | Waring | write_cache()] Error in write_cache(). Working without cahce. (%s)\n", cache_fast[id]->url);
                    status_cache[id] = -200;
                    cache_fast[id]->status = -1;
                    if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0)
                    {
                        finish_connection((struct pollfd*)polls, id);
                    }
                    return;
                }
            }
            else //Не нашли быструю запись в кэше => работаем без кэша
            {
                status_cache[id] = -200;
                printf("[S->C | CACHE | Waring] Fast cache error. Working without cache. (%s)\n", cache_fast[id]->url);
                if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0) //делаем первую отправку
                {
                    finish_connection((struct pollfd*)polls, id);
                }
                return;
            }
            
            if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0) //делаем первую отправку
            {
                cache_fast[id]->status = 2;
            }
            return;
        }
        else //Это не первое вхождение
        {
            if (status_cache[id] == -200) //Режим работы без кэша
            {
                if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0)
                {
                    finish_connection((struct pollfd*)polls, id);
                }
                return;
            }
            else if (status_cache[id] < 0) //Сюда вероятно никогда не зайдём... Если и попадём, то работаем без кэша
            {
                printf("[S->C | CACHE | Waring] Suda verojatno ne popadu nikoga?????????????? (%s)\n", cache_fast[id]->url);
                status_cache[id] = -200; //произошла ошибка кэша, надо продолжить просто отправлять данне
                if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0) //далее работаем без кэша
                {
                    finish_connection((struct pollfd*)polls, id);
                }
                return;
            }
            else //Работаем с кэшом
            {
                int write_cache_size = write_cache(cache_fast[id], id, buf, size_buf_read);
                if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                {
                    status_cache[id] = -200;
                    cache_fast[id]->status = -1;
                    printf("[S->C | CACHE | Waring | write_cache()] Working without cache. (%s)\n", cache_fast[id]->url);
                }
                if (cache_fast[id]->status == 2) //Сокет у клиента закрылся, пытаемся докачать кэш до конца
                {
                    return;
                }
                if (sender_SC((struct pollfd*)polls, id, buf, size_buf_read) < 0) //Ошибка отправки к клиенту данных, тогда далее будем пытаться докачать кэш
                {
                    cache_fast[id]->status = 2;
                }
                return;
            }
        }
    }
    else //Завершение соединения, т.к. recv() = 0
    {
        printf("[S->C | finish] recv() = %d; ", size_buf_read);
        if (cache_fast[id] != NULL)
        {
            printf("(%s); ", cache_fast[id]->url);
            cache_fast[id]->status = 0;
        }
        if (status_cache[id] > 0)
        {
            printf("cache writed = %d. ", status_cache[id]);
        }
        else
        {
            printf("working without cache. ");
        }
        finish_connection((struct pollfd*)polls, id);
    }
}

void sigExit(int signal)
{
    signalExit = 0;
}

//сокет перериспользуемый
int main(int argc, char* argv[])
{
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);
    signal(SIGQUIT, sigExit);
    signal(SIGTRAP, sigExit);

    int port = PORT;
    if (argc != 2)
    {
        printf("Using default port\n");
    }
    else
    {
        port = atoi(argv[1]);
    }

	int proxy_socket;
    if ((proxy_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Server socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address_server;
    address_server.sin_family = AF_INET;
    address_server.sin_addr.s_addr = INADDR_ANY; //addressBuffer
    address_server.sin_port = htons(port);
    int flag = 1;
    setsockopt(proxy_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    if (bind(proxy_socket, (struct sockaddr*)&address_server, sizeof(address_server)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server strating at port %d \n", port);

    if (listen(proxy_socket, SOMAXCONN) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    size_t addrlen = sizeof(address_server);

    struct sockaddr_in clientAddress;
    unsigned int clientAddressLength = sizeof(clientAddress);

    for (int i = 0; i < MAX_CONNECTIONS * 2 + 1; i++)
    {
        polls[i].events = POLLIN; //POLLIN - Можно считывать данные
    }
    for (int i = 0; i < MAX_CONNECTIONS * 2; i++)
    {
        polls[i].fd = -1; 
    }
    polls[MAX_CONNECTIONS * 2].fd = proxy_socket;

    init_parsers_request();
    init_parsers_response();

    cache_list = init_cache_head();

    while (signalExit)
    {
        int readyPolls = -99999;
        if ((clients_cache <= 0) || (connections > 0))
        {
            readyPolls = poll((struct pollfd*)&polls, MAX_CONNECTIONS * 2 + 1, -1);
        }
        if ((readyPolls == -1) || (readyPolls == 0))
        {
            close(proxy_socket);
            printf("[MAIN | Waring] Error in poll server\n");
            break;
        }

        if (polls[MAX_CONNECTIONS * 2].revents == POLLIN) //кладет в полл и всё (без коннекта к серверку)
        {
            polls[MAX_CONNECTIONS * 2].revents = 0;
            int freePoll = findFreePoll(polls);
            if (freePoll == -1)
            {
                printf("[MAIN | Waring] All polls is using. Waiting for new free descriptor\n");
                continue;
            }

            int clientFD = accept(proxy_socket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientFD < 0)
            {
                printf("[MAIN | Waring] Error in client accept\n");
                continue;
            }
            polls[freePoll].fd = clientFD;
        }
        // чекаем от клиента к серверу
        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            fromClient_toServer((struct pollfd*)&polls, i, cache_list); //союираем весь запрос полностью и отпрвляем на сервер ии больше ничего не делаем
        }
        //чекаем от серкера к клиенту
        for (int i = MAX_CONNECTIONS; i < MAX_CONNECTIONS + MAX_CONNECTIONS; i++) //connections
        {
            fromServer_toClient((struct pollfd*)&polls, i, cache_list);
        }
        //чистим кэш
        if (size_cache_list > 100)
        {
            int check = 0;
            for (int i = 0; i < MAX_CONNECTIONS * 2; i++)
            {
                if (polls[i].fd != -1)
                {
                    check = 1;
                    break;
                }
            }
            if (check == 0)
            {
                printf("--------------------------------------------------------------------------------------------------------------------\n");
                printf("[CACHE | clear()] Clear %d sites; ", size_cache_list / 10);
                clear_part_cache(cache_list, size_cache_list / 10);
                printf("\n");
                printf("--------------------------------------------------------------------------------------------------------------------\n");
            }
        }
    }

    printf("Exiting... Cache ");
    clear_cache_from(cache_list); //Почистим весь кэш
    shutdown(proxy_socket, SHUT_RDWR);
    close(proxy_socket);
    for (int i = 0; i < MAX_CONNECTIONS; i++) //На всякий случай почистим ещё URLs
    {
        free(cache_URLs[i]);
    }
    for (int i = 0; i < MAX_CONNECTIONS * 2; i++) //Почистим все дескрипторы поллов (на всякий случай)
    {
        close(polls[i].fd);
    }
    printf("\n");
    return 0;
}