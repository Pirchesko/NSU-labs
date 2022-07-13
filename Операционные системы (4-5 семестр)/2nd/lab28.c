#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include <netdb.h>
#include <unistd.h>
#include "stdlib.h"
#include "errno.h"

#define MAX_STRING 25
#define SIZE_BUF 4096

typedef struct {
    char* host_name;
    int port;
    char* path;
} url_info;

url_info* url_parser(char* url)
{
    url_info* info = (url_info*)malloc(sizeof(url_info));
    int start, end, port_exist = 0;
    info->port = 80;
    info->path = NULL;
    if (strstr(url, "http://"))
    {
        start = 7;
    }
    else
    {
        if (strstr(url, "https://"))
        {
            start = 8;
        }
        else
        {
            free(info);
            return NULL;
        }
    }
    end = start;
    while ((end < strlen(url)) && (url[end] != '/'))
    { 
        if (url[end] == ':')
        {
            port_exist = 1;
            break;
        }
        end++;
    }
    info->host_name = (char*)malloc(sizeof(char) * (end - start) + 1);
    strncpy(info->host_name, url + start, end - start);
    if (port_exist)
    {
        int port = 0;
        end += 1;
        while ((url[end] >= '0') && (url[end] <= '9') && (end < strlen(url)))
        {
            port = port * 10 + url[end] - '0';
            end++;
        }
        info->port = port;
    }
    start = end;
    if (start != strlen(url))
    {
        info->path = (char*)malloc(sizeof(char) * (strlen(url) - start) + 1);
        strncpy(info->path, url + start, strlen(url) - start);
    }
    printf("Host: %s\n", info->host_name);
    printf("Port: %d\n", info->port);
    if (info->path != NULL)
    {
        printf("Path: %s\n\n", info->path);
    }
    else
    {
        printf("Path: no path\n\n");
    }
    return info;
}

char* make_headers(url_info* info)
{
    char* head;
    int size = strlen(info->host_name) + strlen("GET  HTTP/1.0\r\nHost: ") + 5;
    if (info->path != NULL)
    {
        size += strlen(info->path);
    }
    else
    {
        size += 1;
    }
    head = (char*)malloc(sizeof(char) * size);
    memset(head, 0, size);
    strcat(head, "GET ");
    if (info->path != NULL)
    {
        strcat(head, info->path);
    }
    else
    {
        strcat(head, "/");
    }
    strcat(head, " HTTP/1.0\r\nHost: ");
    strcat(strcat(head, info->host_name), "\r\n\r\n");
    printf("%s\n", head);
    return head;
}

int open_connection(url_info* info)
{
    int socket_fd;
    struct addrinfo* hints, * res, * rp;
    char str_port[11];
    int result;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    snprintf(str_port, 11, "%d", info->port);

    if ((result = getaddrinfo(info->host_name, str_port, hints, &res)))
    {
        errno = result;
        perror("getaddrinfo");
        free(hints);
        exit(0);
    }
    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_fd == -1)
        {
            continue;
        }
        if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }
        close(socket_fd);
        free(hints);
        freeaddrinfo(res);
        free(rp);
        perror("socket");
        exit(0);
    }
    free(hints);
    freeaddrinfo(res);
    free(rp);
    return socket_fd;
}

void urlViewer(char* url)
{
    int socket_fd;
    char* headers;
    int result;
    int count_string = 0;
    int end = 0;
    int top = 0;
    int down = 0;
    char buf[SIZE_BUF];

    url_info* info = url_parser(url);
    if (info == NULL)
    {
        perror("Bad url");
        return;
    }
    socket_fd = open_connection(info);
    headers = make_headers(info);
    send(socket_fd, headers, strlen(headers), 0);

    while (1)
    {
        //printf("New while\n");
        //printf("top = %d\ndown = %d\ncount_string = %d\nend = %d\n", top, down, count_string, end);
        fd_set reads, writes;
        FD_ZERO(&reads); //обнул€ем структуры
        FD_ZERO(&writes);
        if (count_string >= MAX_STRING) // ак только у нас уже выведено больше 25 строк мы можем начать реагировать на нажатие клавиши
        {
            //printf("FD_SET for keyboard\n");
            FD_SET(0, &reads); //0 - это клавиатура
            //write(1, "\r                       \r", 25); //Ёта штука не работает после select()
        }
        if (top != SIZE_BUF && !end) //≈сли у нас есть свободное место в буфере, то будем считывать данные с сокета
        {
            //printf("FD_SET for socket\n");
            FD_SET(socket_fd, &reads);
        }
        //printf("Select.....\n");
        result = select(socket_fd + 1, &reads, &writes, NULL, NULL); //Select скажет на каких сетах произошло событие
        //printf("Selected: %d\n", result);
        if (result < 0)
        {
            perror("select");
            close(socket_fd);
            return;
        }
        if (result > 0) //” нас произошло какое-то собитие
        {
            //printf("Select > 0\n");
            if (top != SIZE_BUF && FD_ISSET(socket_fd, &reads)) //” есть место в буфере и произошло событие на сокете
            {
                //printf("A lot of free space! Recieving\n");
                result = recv(socket_fd, buf + top, SIZE_BUF - top, 0);
                if (result == 0)
                {
                    end = 1;
                }
                if (result == -1)
                {
                    perror("recv");
                    close(socket_fd);
                    return;
                }
                //printf("recv = %d\n", result);
                //printf("top = %d; old top = %d\n", top + result, top);
                top += result;
            }
            if (FD_ISSET(0, &reads) && count_string >= MAX_STRING) //ћы уже вывели 25 строчек и теперь считываем нажатие клавиатуры (при этом произошло событие на клаве)
            {
                //printf("FD_ISSET on keyboard\n");
                char tmp[64];
                read(0, tmp, 64);
                count_string = 0;
            }
        }

        if (count_string < MAX_STRING && top != down) //≈сли мы ещЄ не вывели (или не до конца вывели) 25 строк и у нас есть что вообще выводить
        {
            int tmp = 0;
            //printf("Not all strings printed and free buff\n");
            while(count_string < MAX_STRING && down + tmp < top)
            {
                if (buf[down + tmp] == '\n')
                {
                    count_string++;
                }
                tmp++;
            }
            //printf("tmp = %d\ncount_string = %d\n", tmp, count_string);
            write(1, buf + down, tmp);
            //printf("Writed in terminal: down = %d\n", down);
            if (count_string == MAX_STRING)
            {
                write(1, "Press Enter to continue", 23);
            }
            down += tmp;
            //printf("down = %d\n", tmp);
        }

        if (top == down) //¬сЄ что было в буфере - вывели
        {
            //printf("top == down = %d = %d\n", top, down);
            top = 0;
            down = 0;
            if (end)
            {
                break;
            }
        }
    }
    close(socket_fd);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Please, input URL\n");
        return 0;
    }
    urlViewer(argv[1]);
    return 0;
}