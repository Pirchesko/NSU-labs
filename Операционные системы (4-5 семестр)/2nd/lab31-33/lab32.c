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
#include <pthread.h>
#include <errno.h>

#include "http_parser.h"

#define PORT 55555
#define MAX_CONNECTIONS 100
#define BUF 1024

#define ERROR_505 "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n"
#define ERROR_504 "HTTP/1.1 504 Gateway Timeout\r\n\r\n"
#define ERROR_405 "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
#define ERROR_403 "HTTP/1.1 403 Forbidden\r\n\r\n"
#define ERROR_400 "HTTP/1.1 400 Bad request\r\n\r\n"

int connections = 0; //количество соединений (склиент-сервер и сервер-клиент)
int signalExit = 1;
int recv_errors = 0; //debug: остсеживание получения пустых recv() с клиентов
int clients_cache = 0; //счётчик клиентов, получающие данных из кэша
int threads = 0; //счётчик рабочих потоков
int proxy_socket;

// -------------------------------------- HTTP parser -----------------------------------------------

http_parser_settings p_sett_request;
http_parser request_parser;
http_parser_settings p_sett_response;
http_parser response_parser;
pthread_mutex_t mutex_request;
pthread_mutex_t mutex_response;

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
    info->method = (char*)calloc(strlen(http_method_str((http_method)parser->method)) + 1, sizeof(char));
    if (info->method == NULL)
    {
        return -1;
    }
    strcpy(info->method, http_method_str((http_method)parser->method));
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
    pthread_mutex_t mutex;
} Cache;

Cache* cache_list;
int size_cache_list = 0; //Счётчик кэш-записей в кэше

void lock_mutex(pthread_mutex_t* mutex)
{
    if (pthread_mutex_lock(mutex))
    {
        printf("Error locking mutex\n");
    }
}

void unlock_mutex(pthread_mutex_t* mutex)
{
    if (pthread_mutex_unlock(mutex))
    {
        printf("Error unlocking mutex\n");
    }
}

Cache* init_cache_head()
{
    Cache* head = (struct Cache*)calloc(1, sizeof(Cache));
    head->url = NULL;
    head->buf = NULL;
    head->next = NULL;
    pthread_mutex_init(&(head->mutex), NULL);
    return head;
}

//Очистка кэша начиная с конкретного указателя
void clear_cache_from(Cache* from)
{
    if (from == NULL)
    {
        return;
    }
    lock_mutex(&(from->mutex));
    Cache* tmpCache;
    int size_cache_free = 0;
    for (Cache* iter = from; iter; iter = tmpCache)
    {
        if (iter->next != NULL)
        {
            lock_mutex(&(iter->next->mutex));
        }
        tmpCache = iter->next;
        size_cache_free += iter->size_buf;
        free(iter->buf);
        free(iter->url);
        unlock_mutex(&(iter->mutex));
        pthread_mutex_destroy(&(iter->mutex));
        free(iter);
        size_cache_list--;
    }
    printf("clear = %d KB; ", size_cache_free / 1024);
}

//Отчитываем с какой записи будем стриать кэш
void clear_part_cache(int count_field_to_free)
{
    if (cache_list == NULL)
    {
        return;
    }
    lock_mutex(&(cache_list->mutex));
    if (cache_list->next != NULL)
    {
        lock_mutex(&(cache_list->next->mutex));
    }
    Cache* tmpCache = cache_list->next;
    unlock_mutex(&(cache_list->mutex));
    int list_position = 0;
    int size_all_cache = 0;
    while ((tmpCache != NULL) && (list_position < size_cache_list - count_field_to_free))
    {
        size_all_cache += tmpCache->size_buf;
        Cache* prev = tmpCache;
        if (tmpCache->next != NULL)
        {
            lock_mutex(&(tmpCache->next->mutex));
        }
        tmpCache = tmpCache->next;
        unlock_mutex(&(prev->mutex));
        list_position++;
    }
    if (tmpCache == NULL)
    {
        printf("[MAIN | Waring] Error in free cache list!\n");
        return;
    }
    Cache* tmpCache_ = tmpCache;
    tmpCache_ = tmpCache->next;
    tmpCache->next = NULL;
    unlock_mutex(&(tmpCache->mutex));
    size_all_cache += tmpCache->size_buf;
    printf("size cache = %d KB; ", size_all_cache / 1024);
    clear_cache_from(tmpCache_);
}

//Посик URL в кэше
Cache* check_url(char* url)
{
    if (url == NULL)
    {
        printf("Error! Url parse is null\n");
        return NULL;
    }
    if (cache_list == NULL)
    {
        return NULL;
    }
    lock_mutex(&(cache_list->mutex));
    if (cache_list->next != NULL)
    {
        lock_mutex(&(cache_list->next->mutex));
    }
    Cache* tmpCache = cache_list->next;
    unlock_mutex(&(cache_list->mutex));
    while (tmpCache != NULL)
    {
        if (strcmp(tmpCache->url, url) == 0)
        {
            unlock_mutex(&(tmpCache->mutex));
            return tmpCache;
        }
        Cache* prev = tmpCache;
        if (tmpCache->next != NULL)
        {
            lock_mutex(&(tmpCache->next->mutex));
        }
        tmpCache = tmpCache->next;
        unlock_mutex(&(prev->mutex));
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
    pthread_mutex_init(&(cache->mutex), NULL);
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
    lock_mutex(&(cache_list->mutex));
    if (cache_list->next != NULL)
    {
        lock_mutex(&(cache_list->next->mutex));
    }
    lock_mutex(&(cache->mutex));
    cache->next = head->next;
    head->next = cache;
    size_cache_list++;
    unlock_mutex(&(cache_list->mutex));
    unlock_mutex(&(cache->mutex));
    if (cache_list->next->next != NULL)
    {
        unlock_mutex(&(cache_list->next->next->mutex));
    }
    return cache;
}

//Запись информации в кэш
int write_cache(Cache* cache, char* buf, int size, int* total_read_size)
{
    if (cache != NULL)
    {
        lock_mutex(&(cache->mutex));
        if (cache->buf == NULL)
        {
            cache->buf = (char*)calloc(size, sizeof(char));
            if (cache->buf == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache - calloc()"); 
                (*total_read_size) = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            (*total_read_size) = size;
            cache->size_buf = (*total_read_size);
            if (memcpy(cache->buf, buf, size) == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache"); 
                (*total_read_size) = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
        }
        else
        {
            cache->buf = (char*)realloc(cache->buf, (*total_read_size) + size);
            if (cache->buf == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache - realloc()");
                //(*total_read_size) = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            (*total_read_size) = (*total_read_size) + size;
            cache->size_buf = (*total_read_size);
            if (memcpy(cache->buf + (*total_read_size) - size, buf, size) == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache"); 
                //(*total_read_size) = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
        }
        //printf("[CACHE | DEBUG] Size buf = %d\n", strlen(cache->buf));
        unlock_mutex(&(cache->mutex));
        return (*total_read_size);
    }
    else
    {
        return -1;
    }
}

//Чтенение информации из кэша
int read_cache(Cache* cache, int id, char* buf, int* was_read)
{
    if (cache != NULL)
    {
        lock_mutex(&(cache->mutex));
        if (cache->buf == NULL)
        {
            unlock_mutex(&(cache->mutex));
            return -1;
        }
        else
        {
            int mem_size = 0;
            int read_size = cache->size_buf - (*was_read);
            if (read_size < BUF)
            {
                mem_size = read_size;
            }
            else
            {
                mem_size = BUF;
            }
            if (memcpy(buf, cache->buf + (*was_read), mem_size) == NULL)
            {
                perror("Error in reading cache");
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            (*was_read) += mem_size;
            unlock_mutex(&(cache->mutex));
            return mem_size;
        }
    }
    else
    {
        return -1;
    }
}

//---------------------------------------------------------------------------------------------------

//Завершение соединения со сторон сервера и клиента
void finish_connection(int clientFD, int serverFD)
{
    close(serverFD);
    close(clientFD);
    //free(cache_URLs[id - MAX_CONNECTIONS]);
    //cache_URLs[id - MAX_CONNECTIONS] = NULL;
    //cache_fast[id] = NULL;
    connections--;
    threads--;
    printf("Connection dropped: clientFD = %d; serverFD = %d; connections = %d; threads = %d (after drop)\n", clientFD, serverFD, connections, threads);
}

//Подключение к серверу (url - сайтам)
int open_connection(struct request_info* info)
{
    int socket_fd = -1;
    int op = fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    struct addrinfo* hints, * res, * rp;
    char str_port[11];
    int result;
    hints = (addrinfo*)calloc(1, sizeof(struct addrinfo));
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
        ((sockaddr_in*)res->ai_addr)->sin_port = htons(80);

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
void bad_errors(int clientFD, int error)
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
        int size_buf = strlen(text_error);
        sprintf(buf, "%s", text_error);//! \0
        int size_buf_write = send(clientFD, buf, size_buf, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
        if (size_buf_write != size_buf)
        {
            printf("[WARING] ERROR! Error in_bad_errors\n");
        }
    }
    close(clientFD);
    threads--;
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
    memmove(s + 5, newpos, size_3);
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
void write_from_cache(Cache* cache_fast, int clientFD)
{
    int total_read_cache = 0;
    while (1)
    {
        char buf[BUF] = { 0 };
        //printf("Read_cache(): %s\n", cache_fast->url);
        int read_cache_size = read_cache(cache_fast, 0, buf, &total_read_cache);
        if (read_cache_size >= 0)
        {
            if (read_cache_size == 0) //прочитали весь кэш и можем завершать соединение
            {
                if (cache_fast->status > 0) //но это оказался не весь кэш, поэтому ждём (был бы весь, статус = 0 оставили)
                {
                    continue;
                }
                printf("[CACHE | C->S | Finish send cache] FD = %d; read_size = %d; total_read = %d (%s)\n", clientFD, read_cache_size, total_read_cache, cache_fast->url);
                clients_cache--;
                threads--;
                close(clientFD);
                //polls[id].fd = -1;
                //status_cache[id] = 0;
                cache_fast = NULL; //?????
                return;
            }
            //total_read_cache += read_cache_size;
            int size_buf_write = send(clientFD, buf, read_cache_size, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
            if (size_buf_write != size_buf_write)
            {
                printf("[CACHE | C->S | write()] ERROR! Sizes of read cache and send is not equals! (%s)\n", cache_fast->url);
                //завершать соединение???
                clients_cache--;
                bad_errors(clientFD, -1);
                return;
            }
        }
        else
        {
            printf("[CACHE | C->S | read() ] ERROR! read_cache() have error! (%s)\n", cache_fast->url);
            //завершать соединение???
            clients_cache--;
            bad_errors(clientFD, -1);
            return;
        }
    }
}

//Отправка данных от сервера к клиенту
int sender_SC(int clientFD, char* buf, int size_buf_read)
{
    int size_buf_write = send(clientFD, buf, size_buf_read, MSG_NOSIGNAL);//write(polls[id - MAX_CONNECTIONS].fd, buf, size_buf_read);
    if (size_buf_write != size_buf_read)
    {
        printf("[S->C | Waring | send] Error in send().\n"/*, cache_fast[id]->url*/);
        //finish_connection((struct pollfd*)polls, id);
        return -1;
    }
    return size_buf_write;
}

//Обработчик севрер-клиент (->)
void server_client_handler(int clientFD, int serverFD, Cache* cache_fast, char* cache_URL)
{
    struct pollfd poll_server;
    poll_server.events = POLLIN;
    poll_server.fd = serverFD;
    int total_read_size = 0;

    while (1)
    {
        char buf[BUF];
        int poll_return = poll((struct pollfd*)&poll_server, 1, -1);
        if (poll_return <= 0)
        {
            printf("[S->C | Waring | poll()] Poll server bad return: %d; id = %d", poll_return, serverFD);
        }
        int size_buf_read = recv(serverFD, buf, sizeof(buf), MSG_NOSIGNAL); //read(polls[id].fd, buf, sizeof(buf));
        errno = 0;
        if (size_buf_read == -1)
        {
            char* errorbuf = strerror(errno);
            fprintf(stderr, "[S->C | Waring | recv()=-1]Error message : %s\n", errorbuf);
        }

        if (size_buf_read > 0)
        {
            if (total_read_size == 0) //Первое вхождение => парсим заголовок ответа и выясняем надо ли кэшировать
            {
                int skip_caching = 0;
                lock_mutex(&mutex_response);
                http_parser_init(&response_parser, HTTP_RESPONSE);
                struct response_info* response_info = (struct response_info*)calloc(1, sizeof(struct response_info));
                if (response_info == NULL) //ошибка выделения памяти
                {
                    unlock_mutex(&mutex_response);
                    free(response_info);
                    printf("[S->C | Waring | response_info] Error in calloc() for status_code. Working without cahce. \n"/*, cache_fast->url*/);
                    skip_caching = 1;
                }

                response_parser.data = response_info;
                size_t parse_size = http_parser_execute(&response_parser, &p_sett_response, buf, (size_t)size_buf_read);
                unlock_mutex(&mutex_response);
                if (parse_size != size_buf_read) //Неправильно распарсен ответ
                {
                    printf("[S->C | Waring | parser_execute] Parser error. Don't know status_code. Working without cahce. \n"/*, cache_fast->url*/); 
                    skip_caching = 1;
                }

                if ((skip_caching == 1) || (strcmp(response_info->status_code, "OK") != 0)) //кэшируем только 200 OK
                {
                    total_read_size = -200;
                    if (sender_SC(clientFD, buf, size_buf_read) < 0) //работаем без кэша
                    {
                        finish_connection(clientFD, serverFD);
                        return;
                    }
                    continue;
                }

                //Начинаем кэширование
                if (cache_URL != NULL) //Быстрая кэш запись есть => можем добалять в кэш
                {
                    cache_fast = create_cache(cache_list, cache_URL, strlen(cache_URL));
                    if (cache_fast == NULL) //Кэш-запись не была создана, работаем без кэша
                    {
                        printf("[S->C | CACHE | Waring | create_cache()] Error in create cache. Working without cahce. (%s)\n", cache_fast->url);
                        total_read_size = -200;
                        if (sender_SC(clientFD, buf, size_buf_read) < 0)
                        {
                            finish_connection(clientFD, serverFD);
                            return;
                        }
                        continue;
                    }
                    cache_fast->status = 1;
                    printf("[C->S | CACHE | start caching] Caching! serverrFD = %d; (%s)\n", serverFD, cache_fast->url);
                    int write_cache_size = write_cache(cache_fast, buf, size_buf_read, &total_read_size);
                    if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                    {
                        printf("[S->C | CACHE | Waring | write_cache()] Error in write_cache(). Working without cahce. (%s)\n", cache_fast->url);
                        total_read_size = -200;
                        cache_fast->status = -1;
                        if (sender_SC(clientFD, buf, size_buf_read) < 0)
                        {
                            finish_connection(clientFD, serverFD);
                            return;
                        }
                        continue;
                    }
                    //total_read_size += write_cache_size;
                }
                else //Не нашли быструю запись в кэше => работаем без кэша
                {
                    total_read_size = -200;
                    printf("[S->C | CACHE | Waring] Fast cache error. Working without cache. (%s)\n", cache_fast->url);
                    if (sender_SC(clientFD, buf, size_buf_read) < 0) //делаем первую отправку
                    {
                        finish_connection(clientFD, serverFD);
                        return;
                    }
                    continue;
                }

                if (sender_SC(clientFD, buf, size_buf_read) < 0) //делаем первую отправку
                {
                    cache_fast->status = 2;
                }
                continue;
            }
            else //Это не первое вхождение
            {
                if (total_read_size == -200) //Режим работы без кэша
                {
                    if (sender_SC(clientFD, buf, size_buf_read) < 0)
                    {
                        finish_connection(clientFD, serverFD);
                        return;
                    }
                    continue;
                }
                else if (total_read_size < 0) //Сюда вероятно никогда не зайдём... Если и попадём, то работаем без кэша
                {
                    printf("[S->C | CACHE | Waring] Suda verojatno ne popadu nikoga %d ?????????????? (%s)\n", total_read_size, cache_fast->url);
                    total_read_size = -200; //произошла ошибка кэша, надо продолжить просто отправлять данне
                    if (sender_SC(clientFD, buf, size_buf_read) < 0) //далее работаем без кэша
                    {
                        finish_connection(clientFD, serverFD);
                        return;
                    }
                    continue;
                }
                else //Работаем с кэшом
                {
                    int write_cache_size = write_cache(cache_fast, buf, size_buf_read, &total_read_size);
                    if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                    {
                        total_read_size = -200;
                        cache_fast->status = -1;
                        printf("[S->C | CACHE | Waring | write_cache()] Working without cache. (%s)\n", cache_fast->url);
                    }
                    //total_read_size += write_cache_size;
                    if (cache_fast->status == 2) //Сокет у клиента закрылся, пытаемся докачать кэш до конца
                    {
                        continue;
                    }
                    if (sender_SC(clientFD, buf, size_buf_read) < 0) //Ошибка отправки к клиенту данных, тогда далее будем пытаться докачать кэш
                    {
                        cache_fast->status = 2;
                    }
                    continue;
                }
            }
        }
        else //Завершение соединения, т.к. recv() = 0
        {
            printf("[S->C | finish] recv() = %d; ", size_buf_read);
            if (cache_fast != NULL)
            {
                printf("(%s); ", cache_fast->url);
                cache_fast->status = 0;
            }
            if (total_read_size > 0)
            {
                printf("cache writed = %d. ", total_read_size);
            }
            else
            {
                printf("working without cache. ");
            }
            finish_connection(clientFD, serverFD);
            return;
        }
    }
}

//Обработчик клиент-сервер (->)
void* client_server_handler(void* param)
{
    int clientFD = *((int*)param);
    free((int*)param);

    struct pollfd poll_client;
    poll_client.events = POLLIN;
    poll_client.fd = clientFD;
    int poll_return = poll((struct pollfd*)&poll_client, 1, -1);
    if (poll_return <= 0)
    {
        printf("[C->S | Waring | poll()] Poll client bad return: %d; id = %d\n", poll_return, clientFD);
    }

    char buf[BUF] = { 0 };
    errno = 0;
    int size_buf = recv(clientFD, buf, sizeof(buf), MSG_NOSIGNAL); //read(polls[id].fd, buf, sizeof(buf));
    if (size_buf == -1)
    {
        char* errorbuf = strerror(errno);
        fprintf(stderr, "[C->S | Waring | recv()=-1] Error message : %s; %d; id_thread = %d\n", errorbuf, clientFD, pthread_self());
    }

    if (size_buf > 0) //Первый recv() как правило всегда >0
    {
        int size_buf_KA = delete_keep_alive(buf, size_buf);
        if (size_buf_KA == -1) //Не нашли поля keep-alive или ошибка в замене его на close
        {
            size_buf_KA == size_buf;
            printf("[C->S | Waring | delete_keep_alive()] Keep-alive is not detected or error!\n");
        }

        lock_mutex(&mutex_request);
        http_parser_init(&request_parser, HTTP_REQUEST);
        struct request_info* request_info = (struct request_info*)calloc(1, sizeof(struct request_info));
        if (request_info == NULL) //Ошибка выделения памяти
        {
            unlock_mutex(&mutex_request);
            free(request_info);
            printf("[C->S | Waring | request_info] Error in calloc() for request!\n");
            bad_errors(clientFD, -1);
            pthread_exit(NULL);
        }

        request_parser.data = request_info;
        size_t parse_size = http_parser_execute(&request_parser, &p_sett_request, buf, (size_t)size_buf_KA);
        if (parse_size != size_buf_KA)
        {
            unlock_mutex(&mutex_request);
            printf("[C->S | Waring | http_execute()] HTTPS is not supported! Or maybe parser error. threads = %d (before drop); clientFD = %d; (%s)\n", threads, clientFD, request_info->url);
            bad_errors(clientFD, 403);
            pthread_exit(NULL);
        }

        Cache* cache_fast = check_url(request_info->url); //Проверяем, есть ли в кэше
        if (cache_fast == NULL) //В кэше не обнаружили
        {
            if ((request_parser.http_major != 1) || (request_parser.http_minor != 0))
            {
                unlock_mutex(&mutex_request);
                printf("[C->S | Waring | version http] Bad version HTTP: %d.%d (%s)\n", request_parser.http_major, request_parser.http_minor, request_info->url);
                bad_errors(clientFD, 505);
                pthread_exit(NULL);
            }
            unlock_mutex(&mutex_request);
            //if (strcmp(request_info->method, "GET") != 0) //Можно обрабатывать только GET
            //{
            //    printf("[WARING] Bad method HTTP: %s\n", request_info->method);
            //    bad_errors((struct pollfd*)polls, id, 405);
            //    return;
            //}

            int serverFD = open_connection(request_info); //Подключаемся к серверу
            if (serverFD <= 0)
            {
                printf("[C->S | Waring | connect()] Error in open connetcion: 504 Tiomeout; (%s)\n", request_info->url);
                bad_errors(clientFD, 504);
                pthread_exit(NULL);
            }

            printf("[C->S] Create connection!\t Client: FD = %d; size = %d\t Server: FD = %d; id_thread = %d; (%s)\n", clientFD, size_buf, serverFD, pthread_self(), request_info->url);
            connections++;

            int size_buf_write = send(serverFD, buf, size_buf, MSG_NOSIGNAL); //write(polls[id + MAX_CONNECTIONS].fd, buf, size_buf);
            if (size_buf_write != size_buf)
            {
                printf("[C->S | Waring | send()] Error in send from Client to Server request! (%s)\n", request_info->url);
                bad_errors(clientFD, -1);
                pthread_exit(NULL);
            }

            char* cache_URL = (char*)calloc(strlen(request_info->url) + 1, sizeof(char));
            if (strcpy(cache_URL, request_info->url) == NULL)
            {
                free(cache_URL);
                printf("[C->S | Waring | send()] Error in remember url for cache (url calloc) (%s)\n", request_info->url);
                //return;
            }

            server_client_handler(clientFD, serverFD, cache_fast, cache_URL);
            pthread_exit(NULL);
        }
        else //Нашлась кэш-запись, начинаем работу с кэшем
        {
            unlock_mutex(&mutex_request);
            //send() recv() in while + cache
            clients_cache++;
            write_from_cache(cache_fast, clientFD);
            pthread_exit(NULL);
        }
    }
    else //Первый recv() и он меньше нуля => какая-то ошибка
    {
        printf("[C->S | Waring | recv()] (Close connection) Error in recv()=0 from Client to connect to the server!, FD = %d; size_buf = %d; threads = %d (before drop)\n", clientFD, size_buf, threads);
        recv_errors++;
        bad_errors(clientFD, -1);
        pthread_exit(NULL);
    }
}

void sigExit(int signal)
{
    signalExit = 0;
    printf("Exiting... Cache ");
    clear_cache_from(cache_list); //Почистим весь кэш
    shutdown(proxy_socket, SHUT_RDWR);
    close(proxy_socket);
    pthread_mutex_destroy(&mutex_request);
    pthread_mutex_destroy(&mutex_response);
    printf("\n");
}

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

    struct sockaddr_in clientAddress;
    unsigned int clientAddressLength = sizeof(clientAddress);

    init_parsers_request();
    init_parsers_response();
    pthread_mutex_init(&mutex_request, NULL);
    pthread_mutex_init(&mutex_response, NULL);

    cache_list = init_cache_head();

    struct pollfd poll_proxy;
    poll_proxy.events = POLLIN;
    poll_proxy.fd = proxy_socket;

    while (signalExit)
    {
        int new_client_proxy = poll((struct pollfd*)&poll_proxy, 1, -1);
        if ((new_client_proxy == -1) || (new_client_proxy == 0))
        {
            close(proxy_socket);
            printf("[MAIN | Waring] Error in poll server\n");
            break;
        }

        if ((threads == 0) && (size_cache_list > 100))
        {
            printf("--------------------------------------------------------------------------------------------------------------------\n");
            printf("[CACHE | clear()] Clear %d sites; ", size_cache_list / 10);
            clear_part_cache(size_cache_list / 10);
            printf("\n");
            printf("--------------------------------------------------------------------------------------------------------------------\n");
        }

        if (poll_proxy.revents == POLLIN)
        {
            poll_proxy.revents = 0;
            int clientFD = -1;
            clientFD = accept(proxy_socket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientFD < 0)
            {
                printf("[MAIN | Waring] Error in client accept");
                continue;
            }

            pthread_t thread;
            int* clientFD_ = (int*)malloc(sizeof(int));
            (*clientFD_) = clientFD;
            int error_code = pthread_create(&thread, NULL, client_server_handler, clientFD_);
            if (error_code != 0)
            {
                char buf[256];
                printf("[MAIN | Waring] Error in create thread.");
                continue;
            }
            threads++;
            //printf("[MAIN | Info] Now threads: %d; clientFD = %d\n", threads, clientFD);
        }
    }

    printf("Exiting... Cache ");
    clear_cache_from(cache_list); //Почистим весь кэш
    shutdown(proxy_socket, SHUT_RDWR);
    close(proxy_socket);
    pthread_mutex_destroy(&mutex_request);
    pthread_mutex_destroy(&mutex_response);
    printf("\n");
    return 0;
}