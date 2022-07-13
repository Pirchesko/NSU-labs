#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/devpoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_LEFT 49152
#define PORT_RIGHT 65535
#define MAX_CONNECTIONS_NUMBER 510

struct pollfd polls[MAX_CONNECTIONS_NUMBER * 2 + 2];
int listening;
int clients_num = 0;
struct sockaddr* clientsAddr = NULL;
struct sockaddr* clientsAddrLens = NULL;

void errorInWrinting(struct pollfd* polls, int id)
{
    polls[id].fd = -1;
    polls[id - MAX_CONNECTIONS_NUMBER].fd = -1;
    close(polls[id].fd);
    close(polls[id - MAX_CONNECTIONS_NUMBER].fd);
    clients_num--;
}

int findFirstFreeDesc(struct pollfd* clientsEvDescriptors)
{
    for (int i = 0; i < MAX_CONNECTIONS_NUMBER; ++i)
    {
        if (clientsEvDescriptors[i].fd == -1)
            return i;
    }
    return -1;
}

void correctExit(int sig)
{
    if (sig == SIGINT)
    {
        shutdown(listening, SHUT_RDWR); 
        close(listening);
        free(clientsAddr);
        free(clientsAddrLens);
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    signal(SIGINT, correctExit);
    if (argc != 5)
    {
        printf("Input: <listening ip-address> <listening port> <ip-address translation> <port translation>\n");
        exit(-1);
    }
    char* ip_s = argv[1]; //local ip (ifconfig) 10.4.0.68
    char* port_s = argv[2]; //local port 55555
    char* server_ip_s = argv[3]; //ip (dig parallels.nsu.ru) 84.237.52.62
    char* server_port_s = argv[4]; //80
    if (ip_s == NULL)
    {
        printf("Error in <listening ip-address>\n");
        return 0;
    }
    if (port_s == NULL)
    {
        printf("Error in <listening port>\n");
        return 0;
    }
    if (server_ip_s == NULL)
    {
        printf("Error in <ip-address translation>\n");
        return 0;
    }
    if (server_port_s == NULL)
    {
        printf("Error in <port translation>\n");
        return 0;
    }
    unsigned short listening_port = strtoul(port_s, NULL, 10);
    if ((listening_port < PORT_LEFT) || (listening_port > PORT_RIGHT))
    {
        printf("Port must be in [%d;%d]\n", PORT_LEFT, PORT_RIGHT);
        return 0;
    }
    unsigned short server_port = strtoul(server_port_s, NULL, 10);

    if ((listening = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error in create socket for listening");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_s);
    addr.sin_port = htons(listening_port);
    if (bind(listening, (struct sockaddr*)&addr, sizeof(addr)) == -1) //привязывает к сокету локальный адрес длиной
    { 
        perror("Error in bind for listening");
        return 0;
    }
    if (listen(listening, MAX_CONNECTIONS_NUMBER) == -1) //Параметр задает максимальную длину, до которой может расти очередь ожидающих соединений
    { 
        perror("Error in listen for listening");
        return 0;
    }
    printf("Translation server started successfully!\n");

    struct sockaddr_in addr_target;
    addr_target.sin_family = AF_INET;
    addr_target.sin_addr.s_addr = inet_addr(server_ip_s);
    addr_target.sin_port = htons(server_port); //преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов
    for (int i = 0; i < MAX_CONNECTIONS_NUMBER * 2 + 2; ++i)
    {
        polls[i].events = POLLIN; //POLLIN - Можно считывать данные
    }
    for (int i = 0; i < MAX_CONNECTIONS_NUMBER * 2; ++i)
    {
        polls[i].fd = -1;
    }
    polls[MAX_CONNECTIONS_NUMBER * 2 + 1].fd = listening;

    clientsAddr = (struct sockaddr*)malloc(MAX_CONNECTIONS_NUMBER * sizeof(struct sockaddr));
    clientsAddrLens = (struct sockaddr*)malloc(MAX_CONNECTIONS_NUMBER * sizeof(struct sockaddr));
    while (1)
    {
        int respondClients = poll((struct pollfd*)&polls, MAX_CONNECTIONS_NUMBER * 2 + 2, 100); //(возвращается положительное значение, равное количеству структур с ненулевыми полями revents) количество готовых дескрипторов, 0 в случае тайм - аута, -1 в случае ошибки
        if ((respondClients == -1) || (respondClients == 0))
        {
            close(listening);
            perror("Error in poll");
            exit(1);
        }
        //чек сервак
        if (polls[MAX_CONNECTIONS_NUMBER * 2 + 1].revents == POLLIN)
        { 
            polls[MAX_CONNECTIONS_NUMBER * 2 + 1].revents = 0;
            if (clients_num < MAX_CONNECTIONS_NUMBER)
            {
                int id;
                if ((id = findFirstFreeDesc(polls)) == -1)
                {
                    printf("No free descriptors\n");
                    continue;
                }
                polls[id].fd = accept(listening, &clientsAddr[clients_num], (socklen_t*)&clientsAddrLens[clients_num]); //возращщает дескриптор клиента
                polls[id + MAX_CONNECTIONS_NUMBER].fd = socket(AF_INET, SOCK_STREAM, 0);
                if (connect(polls[id + MAX_CONNECTIONS_NUMBER].fd, (struct sockaddr*)&addr_target, sizeof(addr_target)) == -1) //установить соединение с сервером
                { 
                    perror("Error in connect");
                    exit(-1);
                }
                clients_num++;
            }
        }
        //чек события клиентов
        for (int i = 0; i < clients_num; ++i) //от клиента к серверу
        {
            if ((polls[i].fd == -1) || (polls[i].revents != POLLIN))
            {
                continue;
            }
            polls[i].revents = 0;
            int num_read;
            char buffer[BUFSIZ];
            if ((num_read = read(polls[i].fd, buffer, BUFSIZ)) > 0)
            {
                if (write(polls[i + MAX_CONNECTIONS_NUMBER].fd, buffer, num_read) != num_read)
                {
                    errorInWrinting((struct pollfd*)&polls, i);
                }
            }
            else
            {
                errorInWrinting((struct pollfd*)&polls, i);
            }
        }
        for (int i = MAX_CONNECTIONS_NUMBER; i < MAX_CONNECTIONS_NUMBER * 2; ++i)
        {
            if ((polls[i].fd == -1) || (polls[i].revents != POLLIN))
            {
                continue;
            }
            polls[i].revents = 0;
            int num_read;
            char buffer[BUFSIZ];
            if ((num_read = read(polls[i].fd, buffer, BUFSIZ)) > 0)
            {
                if (write(polls[i - MAX_CONNECTIONS_NUMBER].fd, buffer, num_read) != num_read)
                {
                    errorInWrinting((struct pollfd*)&polls, i);
                }
            }
            else
            {
                errorInWrinting((struct pollfd*)&polls, i);
            }
        }
    }

}

//https://phys.bspu.by/static/um/inf/prg/sem3/book/vcpp/visual/16.html тут интересненько, можно почитать

//  struct pollfd {
//      int fd;             /* дескриптор, который нужно проверить */
//      short events;       /* события на дескрипторе, которые нас интересуют */
//      short revents;      /* события, произошедшие на дескрипторе fd */
//  };
//
//  struct sockaddr_in {
//      short int sin_family;           // Address family, AF_INET
//      unsigned short int sin_port;    // Port number
//      struct in_addr sin_addr;        // Internet address
//      unsigned char sin_zero[8];      // Same size as struct sockaddr
//  };