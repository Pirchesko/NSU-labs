#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 80
#define EMPTY_STRING "\n"

typedef struct Node {
    char* string;
    struct Node* next;
} Node;

Node* list;
int listSize = 0;
int finish = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void notifySortListener(int sign)
{
    if (sign == SIGINT)
    {
        finish = 1;
        signal(sign, SIG_IGN);
    }
}

void freeList(Node* head)
{
    if (head == NULL)
    {
        return;
    }
    Node* tmpNode;
    for (Node* iter = head; iter; iter = tmpNode)
    {
        tmpNode = iter->next;
        free(iter);
    }
}

void ExitFailure(char* errorMsg)
{
    perror(errorMsg);
    freeList(list);
    pthread_exit(NULL);
}

void lockMutex(pthread_mutex_t* mutexParam)
{
    if (pthread_mutex_lock(mutexParam))
    {
        ExitFailure("Error locking mutexParam");
    }
}

void unlockMutex(pthread_mutex_t* mutexParam)
{
    if (pthread_mutex_unlock(mutexParam))
    {
        ExitFailure("Error unlocking mutexParam");
    }
}

Node* initNode(char* str, size_t size)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL)
    {
        free(node);
        printf("Allocation failed\n");
        return NULL;
    }
    node->next = NULL;
    node->string = malloc(size + 1);
    if (strcpy(node->string, str) == NULL)
    {
        free(node->string);
        free(node);
        perror("Error creating node for input string");
        return NULL;
    }
    return node;
}

void pushFront(Node* head, char* str)
{
    if (head == NULL)
    {
        return;
    }
    Node* newNode = initNode(str, strlen(str));
    if (newNode == NULL)
    {
        return;
    }
    newNode->next = head->next;
    head->next = newNode;
    listSize++;
}

void printList(Node* head)
{
    if (head == NULL)
    {
        return;
    }
    printf("----------------------\n");
    for (Node* iter = head->next; iter; iter = iter->next)
    {
        printf("%s", iter->string);
    }
    printf("----------------------\n");

}

Node* initList()
{
    Node* head = malloc(sizeof(Node));
    head->string = NULL;
    head->next = NULL;
    return head;
}

int isEmptyString(char* buf)
{
    if (!strcmp(EMPTY_STRING, buf))
    {
        printList(list);
        return 1;
    }
    return 0;
}

void getStrings()
{
    char buf[BUF_SIZE + 1];
    list = initList();
    while (finish != 1)
    {
        if (fgets(buf, BUF_SIZE + 1, stdin) == NULL)
        {
            continue;
        }
        lockMutex(&mutex);
        if (!isEmptyString(buf))
        {
            pushFront(list, buf);
        }
        unlockMutex(&mutex);
        fflush(stdout);
    }
}

void swap(char** left, char** right)
{
    char* tmp = *left;
    *left = *right;
    *right = tmp;
}

int compare(char* left, char* right)
{
    int leftLen = strlen(left), rightLen = strlen(right);
    int minLength = (leftLen > rightLen) ? rightLen : leftLen;
    int maxLength = (leftLen < rightLen) ? rightLen : leftLen;
    for (int i = 0; i < minLength; i++)
    {
        if (left[i] != right[i])
        {
            return 2 * (left[i] > right[i]) - 1;
        }
    }
    if (!(maxLength - minLength))
    {
        return 0;
    }
    return 2 * (maxLength == strlen(left)) - 1;
}

void* sort(void* data)
{
    while (1)
    {
        if (finish)
        {
            pthread_exit(NULL);
        }
        sleep(5);
        lockMutex(&mutex);
        int i = 0, j = 0;
        int cnt = 0;
        for (Node* node = list->next; node; node = node->next, i++, j = 0, cnt = 0)
        {
            for (Node* innerNode = list->next; innerNode->next; innerNode = innerNode->next, j++)
            {
                if (compare(innerNode->next->string, innerNode->string) < 0)
                {
                    swap(&(innerNode->next->string), &(innerNode->string));
                    cnt++;
                }
            }
            if (cnt == 0)
            {
                break;
            }
        }
        unlockMutex(&mutex);
    }
}

void createThread()
{
    pthread_t threadId;

    if (pthread_create(&threadId, NULL, sort, NULL))
    {
        ExitFailure("Error creating thread");
    }
    getStrings();
    if (pthread_join(threadId, NULL))
    {
        ExitFailure("Error waiting thread");
    }
    freeList(list);
}

int main()
{
    signal(SIGINT, notifySortListener);
    createThread();
    pthread_exit(NULL);
}
