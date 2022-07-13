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

#include "http_parser.h"

#define PORT 55555
#define MAX_CONNECTIONS 50 //максимальное количество соединений в одном потоке пулла
#define THREAD_POOL 5
#define SIZE_TASKS 100
#define BUF 1024

#define ERROR_505 "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n"
#define ERROR_504 "HTTP/1.1 504 Gateway Timeout\r\n\r\n"
#define ERROR_405 "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
#define ERROR_403 "HTTP/1.1 403 Forbidden\r\n\r\n"
#define ERROR_400 "HTTP/1.1 400 Bad request\r\n\r\n"

int signalExit = 1;
int proxy_socket;
pthread_t* thread;
int thread_pool = THREAD_POOL;

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
    char* line = (char*)calloc((size_t)(length + 1), sizeof(char));
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
    info->status_code = (char*)calloc((length + 1), sizeof(char));
    if (info->status_code == NULL)
    {
        return -1;
    }
    memcpy(info->status_code, at, length);
    info->status_code[length] = '\0';
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
        perror("Error locking mutex\n");
    }
}

void unlock_mutex(pthread_mutex_t* mutex)
{
    if (pthread_mutex_unlock(mutex))
    {
        perror("Error unlocking mutex\n");
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
    Cache* tmpCache = NULL;
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
Cache* create_cache(Cache* head, char* url, int size)
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
    cache->url = (char*)calloc((size_t)(size + 1), sizeof(char));
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
int write_cache(Cache* cache, char* buf, int size, int id, int* status_cache)
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
                status_cache[id] = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            status_cache[id] = size;
            cache->size_buf = status_cache[id];
            if (memcpy(cache->buf, buf, size) == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache");
                status_cache[id] = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
        }
        else
        {
            cache->buf = (char*)realloc(cache->buf, (size_t)(status_cache[id] + size));
            if (cache->buf == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache - realloc()");
                //status_cache[id] = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            status_cache[id] = status_cache[id] + size;
            cache->size_buf = status_cache[id];
            if (memcpy(cache->buf + status_cache[id] - size, buf, size) == NULL)
            {
                free(cache->buf);
                free(cache->url);
                free(cache);
                perror("Error in writing cache");
                //status_cache[id] = -1;
                unlock_mutex(&(cache->mutex));
                return -1;
            }
        }
        unlock_mutex(&(cache->mutex));
        return status_cache[id];
    }
    else
    {
        return -1;
    }
}

//Чтенение информации из кэша
int read_cache(Cache* cache, int id, char* buf, int* status_cache)
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
                perror("Error in reading cache");
                unlock_mutex(&(cache->mutex));
                return -1;
            }
            status_cache[id] += mem_size;
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

//-------------------------------------------- Thread Pool ------------------------------------------

int task_client[SIZE_TASKS];
int size_task_client = 0;
pthread_mutex_t task_mutex;
pthread_cond_t task_cond;

//Добавляем в очередь клиента (защищено мутексам)
void add_task(int clientFD) 
{
    lock_mutex(&task_mutex);
    task_client[size_task_client] = clientFD;
    size_task_client++;
    unlock_mutex(&task_mutex);
    //pthread_cond_signal(&task_cond); 
    pthread_cond_broadcast(&task_cond); //появилась новая задача - оповещаем всем
}

//Вытаскиваем из очереди клиента (не защищено мутексами)
int pop_task()
{
    int clientFD = task_client[0];
    for (int i = 0; i < size_task_client; i++)
    {
        task_client[i] = task_client[i + 1];
    }
    size_task_client--;
    return clientFD;
}

//---------------------------------------------------------------------------------------------------

//Завершение соединения со сторон сервера и клиента
void finish_connection(int thread_id, struct pollfd* polls, int id, int* status_cache, char* cache_URLs[], Cache* cache_fast[], int* connections)
{
    close(polls[id].fd);
    close(polls[id - MAX_CONNECTIONS].fd);
    polls[id].fd = -1;
    polls[id - MAX_CONNECTIONS].fd = -1;
    status_cache[id] = 0;
    status_cache[MAX_CONNECTIONS - id] = 0;
    free(cache_URLs[id - MAX_CONNECTIONS]);
    cache_URLs[id - MAX_CONNECTIONS] = NULL;
    cache_fast[id] = NULL;
    (*connections)--;
    printf("[%d] Connection dropped: id = %d; id = %d; connections = %d (after drop)\n", thread_id, id, id - MAX_CONNECTIONS, (*connections));
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
    hints->ai_family = AF_INET;
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

        struct pollfd poll_connect {
            poll_connect.fd = socket_fd,
            poll_connect.events = POLLOUT
        };
        
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
void bad_errors(int thread_id, struct pollfd* polls, int id, int error)
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
        sprintf(buf, "%s", text_error);
        int size_buf_write = send(polls[id].fd, buf, size_buf, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
        if (size_buf_write != size_buf)
        {
            printf("[%d | WARING] ERROR! Error in_bad_errors\n", thread_id);
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
void write_from_cache(int thread_id, struct pollfd* polls, int id, Cache* cache_head, int* status_cache, Cache* cache_fast[], int* clients_cache)
{
    char buf[BUF] = { 0 };
    int read_cache_size = read_cache(cache_fast[id], id, buf, status_cache);
    if (read_cache_size >= 0)
    {
        if (read_cache_size == 0) //прочитали весь кэш и можем завершать соединение
        {
            if (cache_fast[id]->status > 0) //но это оказался не весь кэш, поэтому ждём (был бы весь, статус = 0 оставили) [случай когда кэш закачан но не полностью]
            {
                return;
            }
            printf("[%d | CACHE | C->S | Finish send cache] id = %d; read_cache_size = %d; total_read = %d (%s)\n", thread_id, id, read_cache_size, status_cache[id], cache_fast[id]->url);
            (*clients_cache)--;
            close(polls[id].fd);
            polls[id].fd = -1;
            status_cache[id] = 0;
            cache_fast[id] = NULL;
            return;
        }
        int size_buf_write = send(polls[id].fd, buf, read_cache_size, MSG_NOSIGNAL);//write(polls[id].fd, buf, size_buf);
        if (size_buf_write != size_buf_write)
        {
            printf("[%d | CACHE | C->S | write()] ERROR! Sizes of read cache and send is not equals! (%s)\n", thread_id, cache_fast[id]->url);
            (*clients_cache)--;
            bad_errors(thread_id, (struct pollfd*)polls, id, -1);
            return;
        }
    }
    else
    {
        printf("[%d | CACHE | C->S | read() ] ERROR! read_cache() have error! (%s)\n", thread_id, cache_fast[id]->url);
        (*clients_cache)--;
        bad_errors(thread_id, (struct pollfd*)polls, id, -1);
        return;
    }
}

//Обработчик клиент-сервер (->)
void fromClient_toServer(int thread_id, struct pollfd* polls, int id, Cache* cache_head, Cache* cache_fast[], int* status_cache, char* cache_URLs[], int* connections, int* clients_cache, int* recv_errors)
{
    if ((polls[id].fd == -1) || (status_cache[id] == -200))
    {
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
                    printf("[%d | C->S | Waring | delete_keep_alive()] Keep-alive is not detected or error!\n", thread_id);
                }

                lock_mutex(&mutex_request);
                http_parser_init(&request_parser, HTTP_REQUEST);
                struct request_info* request_info = (struct request_info*)calloc(1, sizeof(struct request_info));
                if (request_info == NULL) //Ошибка выделения памяти
                {
                    unlock_mutex(&mutex_request);
                    free(request_info);
                    printf("[%d | C->S | Waring | request_info] Error in calloc() for request!\n", thread_id);
                    bad_errors(thread_id, (struct pollfd*)polls, id, -1);
                    return;
                }

                request_parser.data = request_info;
                size_t parse_size = http_parser_execute(&request_parser, &p_sett_request, buf, (size_t)size_buf_KA);
                if (parse_size != size_buf_KA)
                {
                    unlock_mutex(&mutex_request);
                    printf("[%d | C->S | Waring | http_execute()] HTTPS is not supported! Or maybe parser error. (%s)\n", thread_id, request_info->url);
                    bad_errors(thread_id, (struct pollfd*)polls, id, 403);
                    return;
                }

                cache_fast[id] = check_url(request_info->url); //Проверяем, есть ли в кэше
                if ((cache_fast[id] != NULL) && (cache_fast[id]->status == -1))
                {
                    cache_fast[id] = NULL; //В кэше обнаружилась запись, но она бракованная, делаем вид что записи нет (потом она удалится)
                }
                if (cache_fast[id] == NULL) //В кэше записи нет
                {
                    if ((request_parser.http_major != 1) || (request_parser.http_minor != 0))
                    {
                        unlock_mutex(&mutex_request);
                        printf("[%d | C->S | Waring | version hhtp] Bad version HTTP: %d.%d (%s)\n", thread_id, request_parser.http_major, request_parser.http_minor, request_info->url);
                        bad_errors(thread_id, (struct pollfd*)polls, id, 505);
                        return;
                    }
                    unlock_mutex(&mutex_request);
                    //if (strcmp(request_info->method, "GET") != 0) //Можно обрабатывать только GET
                    //{
                    //    printf("[WARING] Bad method HTTP: %s\n", request_info->method);
                    //    bad_errors((struct pollfd*)polls, id, 405);
                    //    return;
                    //}

                    int connect_fd = open_connection(request_info); //Подключаемся к серверу
                    if (connect_fd <= 0)
                    {
                        printf("[%d | C->S | Waring | connect()] Error in open connetcion: 504 Tiomeout; (%s)\n", thread_id, request_info->url);
                        bad_errors(thread_id, (struct pollfd*)polls, id, 504);
                        return;
                    }

                    polls[id + MAX_CONNECTIONS].fd = connect_fd;
                    printf("[%d | C->S] Create connection!\t Client: id = %d; FD = %d; size = %d\t Server: id = %d; FD = %d; (%s)\n", thread_id, id, polls[id].fd, size_buf, id + MAX_CONNECTIONS, polls[id + MAX_CONNECTIONS].fd, request_info->url);
                    (*connections)++;

                    int size_buf_write = send(polls[id + MAX_CONNECTIONS].fd, buf, size_buf, MSG_NOSIGNAL); //write(polls[id + MAX_CONNECTIONS].fd, buf, size_buf);
                    if (size_buf_write != size_buf)
                    {
                        printf("[%d | C->S | Waring | send()] Error in send from Client to Server request! (%s)\n", thread_id, request_info->url);
                        bad_errors(thread_id, (struct pollfd*)polls, id, -1);
                        return;
                    }

                    cache_URLs[id] = (char*)calloc(strlen(request_info->url) + 1, sizeof(char));
                    if (strcpy(cache_URLs[id], request_info->url) == NULL)
                    {
                        free(cache_URLs[id]);
                        printf("[%d | C->S | Waring | send()] Error in remember url for cache (url calloc) (%s)\n", thread_id, request_info->url);
                        return;
                    }
                }
                else //Нашлась кэш-запись, начинаем работу с кэшем
                {
                    unlock_mutex(&mutex_request);
                    (*clients_cache)++;
                    write_from_cache(thread_id, (struct pollfd*)polls, id, cache_head, status_cache, cache_fast, clients_cache);
                }
            }
            else //Первый recv() и он меньше нуля => какая-то ошибка
            {
                printf("[%d | C->S | Waring | recv()] (Close connection) Error in recv()=0 from Client to connect to the server!, id = %d; size_buf = %d\n", thread_id, id, size_buf);
                (*recv_errors)++;
                bad_errors(thread_id, (struct pollfd*)polls, id, -1);
                return;
            }
        }
        else //тут мы не первый раз, а значит рыботаем с кэшом:
        {
            write_from_cache(thread_id, (struct pollfd*)polls, id, cache_head, status_cache, cache_fast, clients_cache);
        }
    }
}

//Отправка данных от сервера к клиенту
int sender_SC(int thread_id, struct pollfd* polls, int id, char* buf, int size_buf_read)
{
    int size_buf_write = send(polls[id - MAX_CONNECTIONS].fd, buf, size_buf_read, MSG_NOSIGNAL);//write(polls[id - MAX_CONNECTIONS].fd, buf, size_buf_read);
    if (size_buf_write != size_buf_read)
    {
        printf("[%d | S->C | Waring | send] Error in send().\n", thread_id);
        return -1;
    }
    return size_buf_write;
}

//Обработчик севрер-клиент (->)
void fromServer_toClient(int thread_id, struct pollfd* polls, int id, Cache* cache_head, Cache* cache_fast[], int* status_cache, char* cache_URLs[], int* connections, int* clients_cache, int* recv_errors)
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
            lock_mutex(&mutex_response);
            http_parser_init(&response_parser, HTTP_RESPONSE);
            struct response_info* response_info = (struct response_info*)calloc(1, sizeof(struct response_info));
            if (response_info == NULL) //ошибка выделения памяти
            {
                unlock_mutex(&mutex_response);
                free(response_info);
                printf("[%d | S->C | Waring | response_info] Error in calloc() for status_code. Working without cahce. (%s)\n", thread_id, cache_fast[id]->url);
                skip_caching = 1;
            }

            response_parser.data = response_info;
            size_t parse_size = http_parser_execute(&response_parser, &p_sett_response, buf, (size_t)size_buf_read);
            unlock_mutex(&mutex_response);
            if (parse_size != size_buf_read) //Неправильно распарсен ответ
            {
                printf("[%d | S->C | Waring | parser_execute] Parser error. Don't know status_code. Working without cahce.\n", thread_id);
                skip_caching = 1;
            }

            if ((skip_caching == 1) || (strcmp(response_info->status_code, "OK") != 0)) //кэшируем только 200 OK
            {
                status_cache[id] = -200;
                if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0) //работаем без кэша
                {
                    finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                }
                return;
            }

            //Начинаем кэширование
            if (cache_URLs[id - MAX_CONNECTIONS] != NULL) //Быстрая кэш запись есть => можем добалять в кэш
            {
                cache_fast[id] = create_cache(cache_head, cache_URLs[id - MAX_CONNECTIONS], strlen(cache_URLs[id - MAX_CONNECTIONS]));
                if (cache_fast[id] == NULL) //Кэш-запись не была создана, работаем без кэша
                {
                    printf("[%d | S->C | CACHE | Waring | create_cache()] Error in create cache. Working without cahce. (%s)\n", thread_id, cache_fast[id]->url);
                    status_cache[id] = -200;
                    if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0)
                    {
                        finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                    }
                    return;
                }
                cache_fast[id]->status = 1;
                printf("[%d | C->S | CACHE | start caching] Caching! id = %d; (%s)\n", thread_id, id, cache_fast[id]->url);
                int write_cache_size = write_cache(cache_fast[id], buf, size_buf_read, id, status_cache);
                if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                {
                    printf("[%d | S->C | CACHE | Waring | write_cache()] Error in write_cache(). Working without cahce. (%s)\n", thread_id, cache_fast[id]->url);
                    status_cache[id] = -200;
                    cache_fast[id]->status = -1;
                    if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0)
                    {
                        finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                    }
                    return;
                }
            }
            else //Не нашли быструю запись в кэше => работаем без кэша
            {
                status_cache[id] = -200;
                printf("[%d | S->C | CACHE | Waring] Fast cache error. Working without cache. (%s)\n", thread_id, cache_fast[id]->url);
                if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0) //делаем первую отправку
                {
                    finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                }
                return;
            }

            if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0) //делаем первую отправку
            {
                cache_fast[id]->status = 2;
            }
            return;
        }
        else //Это не первое вхождение
        {
            if (status_cache[id] == -200) //Режим работы без кэша
            {
                if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0)
                {
                    finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                }
                return;
            }
            else if (status_cache[id] < 0) //Сюда вероятно никогда не зайдём... Если и попадём, то работаем без кэша
            {
                printf("[%d | S->C | CACHE | Waring] Suda verojatno ne popadu nikoga?????????????? (%s)\n", thread_id, cache_fast[id]->url);
                status_cache[id] = -200; //произошла ошибка кэша, надо продолжить просто отправлять данне
                if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0) //далее работаем без кэша
                {
                    finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
                }
                return;
            }
            else //Работаем с кэшом
            {
                int write_cache_size = write_cache(cache_fast[id], buf, size_buf_read, id, status_cache);
                if (write_cache_size < 0) //Ошибка при записи кэша, работаем без кэша
                {
                    status_cache[id] = -200;
                    cache_fast[id]->status = -1;
                    printf("[%d | S->C | CACHE | Waring | write_cache()] Working without cache. (%s)\n", thread_id, cache_fast[id]->url);
                }
                if (cache_fast[id]->status == 2) //Сокет у клиента закрылся, пытаемся докачать кэш до конца
                {
                    return;
                }
                if (sender_SC(thread_id, (struct pollfd*)polls, id, buf, size_buf_read) < 0) //Ошибка отправки к клиенту данных, тогда далее будем пытаться докачать кэш
                {
                    cache_fast[id]->status = 2;
                }
                return;
            }
        }
    }
    else //Завершение соединения, т.к. recv() = 0
    {
        printf("[%d | S->C | finish] recv() = %d; ", thread_id, size_buf_read);
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
        finish_connection(thread_id, (struct pollfd*)polls, id, status_cache, cache_URLs, cache_fast, connections);
    }
}

void* new_thread(void* param)
{
    int thread_id = *((int*)param);
    free((int*)param);
    printf("[ %d | %d | new_thread()] Thread started!\n", thread_id, (int)pthread_self());

    struct pollfd polls[MAX_CONNECTIONS * 2]{};
    int connections = 0; //количество соединений (склиент-сервер и сервер-клиент)
    int recv_errors = 0; //debug: остсеживание получения пустых recv() с клиентов
    int clients_cache = 0; //счётчик клиентов, получающие данных из кэша
    int poll_in_work = 0;

    Cache* cache_fast[MAX_CONNECTIONS * 2] = { 0 }; //Быстрый доступ в кэш для каждого polls[i] (указатель на указатель в кэш)
    int status_cache[MAX_CONNECTIONS * 2] = { 0 };
    // [MAX_CONNECTIONS; MAX_CONNECTIONS*2]: 0 - ничего не происходило; -200 - работаем без кэша; -1 - ошибка при записи кэша; >0 размер записанного кэша на данный момент
    // [0; MAX_CONNECTIONS]: 0 - ничего не происходило; -200 - работаем без кэша (тут не пригодилось); -1 - какая-то ошибка; >0 размер считанного кэша на данный момент
    char* cache_URLs[MAX_CONNECTIONS] = { 0 }; //Быстрый способ добраться до url (выделяется память)

    for (int i = 0; i < MAX_CONNECTIONS * 2; i++)
    {
        polls[i].events = POLLIN;
        polls[i].fd = -1;
    }

    while (signalExit == 1)
    {
        lock_mutex(&task_mutex);
        while ((size_task_client == 0) && (connections == 0) && (clients_cache == 0) && (poll_in_work == 0))
        {
            if (signalExit == 0)
            {
                break;
            }
            pthread_cond_wait(&task_cond, &task_mutex); //ждём если нет никакой обработки
        }

        int clientFD = -1;
        if (size_task_client > 0)
        {
            clientFD = pop_task();
        }
        unlock_mutex(&task_mutex);

        if (clientFD > 0)
        {
            int freePoll = findFreePoll(polls);
            if (freePoll == -1)
            {
                printf("[%d thread | Waring | new_thread()] All polls is using. Push in tasks\n", thread_id);
                add_task(clientFD);
            }
            else
            {
                printf("[%d thread | new_thread()] ClientFD = %d in polls[%d]\n", thread_id, clientFD, freePoll);
                polls[freePoll].fd = clientFD;
            }
        }

        //Обновляем количество рабочих поллов
        poll_in_work = 0;
        for (int i = 0; i < MAX_CONNECTIONS * 2; i++)
        {
            if (polls[i].fd != -1)
            {
                poll_in_work++;
            }
        }

        int readyPolls = -9999;
        if ((connections > 0) || (clientFD != -1))
        {
            readyPolls = poll((struct pollfd*)&polls, MAX_CONNECTIONS * 2, -1);
        }
        if ((readyPolls == -1) || (readyPolls == 0))
        {
            close(proxy_socket);
            printf("[%d thread | Waring] Error in poll server\n", thread_id);
            break;
        }

        //Чекаем от клиента к серверу
        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            fromClient_toServer(thread_id, (struct pollfd*)&polls, i, cache_list, cache_fast, status_cache, cache_URLs, &connections, &clients_cache, &recv_errors); //союираем весь запрос полностью и отпрвляем на сервер
        }
        //Чекаем от серкера к клиенту
        for (int i = MAX_CONNECTIONS; i < MAX_CONNECTIONS + MAX_CONNECTIONS; i++) //connections
        {
            fromServer_toClient(thread_id, (struct pollfd*)&polls, i, cache_list, cache_fast, status_cache, cache_URLs, &connections, &clients_cache, &recv_errors);
        }
    }

    //операции по заверщению потока
    for (int i = 0; i < MAX_CONNECTIONS; i++) //На всякий случай почистим ещё URLs
    {
        free(cache_URLs[i]);
    }
    for (int i = 0; i < MAX_CONNECTIONS * 2; i++) //Почистим все дескрипторы поллов (на всякий случай)
    {
        close(polls[i].fd);
    }
    printf("[%d thread | finish] Thread is finish work\n", thread_id);
    pthread_exit(NULL);
}

void sigExit(int signal)
{
    signalExit = 0;
    printf("Exiting... Cache ");
    clear_cache_from(cache_list); //Почистим весь кэш
    shutdown(proxy_socket, SHUT_RDWR);
    close(proxy_socket);
    printf("\n");
    pthread_cond_broadcast(&task_cond);
    pthread_mutex_destroy(&task_mutex);
    pthread_cond_destroy(&task_cond);
    pthread_mutex_destroy(&mutex_request);
    pthread_mutex_destroy(&mutex_response);

    if (thread != NULL)
    {
        for (int i = 0; i < thread_pool; i++)
        {
            pthread_join(thread[i], NULL);
        }
        free(thread);
    }
}

int main(int argc, char* argv[]) //Первый аргумент - порт, второй - количество потоков в пулле
{
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);
    signal(SIGQUIT, sigExit);
    signal(SIGTRAP, sigExit);

    int port = PORT;
    if (argv[1] == NULL)
    {
        printf("Using default port\n");
    }
    else
    {
        port = atoi(argv[1]);
    }

    if ((argv[2] == NULL) || (argc < 2))
    {
        printf("Using default count of thread_pool\n");
    }
    else
    {
        thread_pool = atoi(argv[2]);
    }

    if ((proxy_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Server socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address_server {};
    address_server.sin_family = AF_INET;
    address_server.sin_addr.s_addr = INADDR_ANY;
    address_server.sin_port = htons(port);
    int flag = 1;
    setsockopt(proxy_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    if (bind(proxy_socket, (struct sockaddr*)&address_server, sizeof(address_server)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server strating at port %d\n", port);

    if (listen(proxy_socket, SOMAXCONN) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientAddress {};
    unsigned int clientAddressLength = sizeof(clientAddress);

    cache_list = init_cache_head();

    pthread_mutex_init(&task_mutex, NULL);
    pthread_cond_init(&task_cond, NULL);

    init_parsers_request();
    init_parsers_response();
    pthread_mutex_init(&mutex_request, NULL);
    pthread_mutex_init(&mutex_response, NULL);

    thread = (pthread_t*)calloc(thread_pool, sizeof(pthread_t));
    for (int i = 0; i < thread_pool; i++)
    {
        int* thread_ID = (int*)malloc(sizeof(int));
        (*thread_ID) = i;
        int error_code = pthread_create(&thread[i], NULL, new_thread, thread_ID);
        if (error_code != 0)
        {
            char buf[256]{ 0 };
            printf("[MAIN | Waring] Error in create thread.");
            continue;
        }
    }

    struct pollfd poll_proxy {
        poll_proxy.fd = proxy_socket,
        poll_proxy.events = POLLIN
    };
    
    while (signalExit)
    {
        int new_client_proxy = poll((struct pollfd*)&poll_proxy, 1, -1);
        if ((new_client_proxy == -1) || (new_client_proxy == 0))
        {
            close(proxy_socket);
            printf("[MAIN | Waring] Error in poll server\n");
            break;
        }

        if (poll_proxy.revents == POLLIN)
        {
            poll_proxy.revents = 0;
            int clientFD = -1;
            clientFD = accept(proxy_socket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientFD < 0)
            {
                printf("[MAIN | Waring] Error in client accept\n");
                continue;
            }

            if (size_cache_list > 100)
            {
                printf("--------------------------------------------------------------------------------------------------------------------\n");
                printf("[CACHE | clear()] Clear %d sites; ", size_cache_list / 10);
                clear_part_cache(size_cache_list / 10);
                printf("\n");
                printf("--------------------------------------------------------------------------------------------------------------------\n");
            }

            //Кидаем задачу в список
            add_task(clientFD);
            printf("[MAIN | add_task] ClientFD = %d pushed in task!\n", clientFD);
        }
    }

    return 0;
}