#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_error(int error_code, char* argv[])
{
    char buf[256];
    strerror_r(error_code, buf, sizeof buf);
    fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
    exit(1);
}

struct ListNode {
    struct Node* next;
    struct Node* prev;
    char* value;
};
typedef struct ListNode ListNode;

ListNode* listHead = NULL;
const int length = 10;
pthread_mutex_t mutex;


void printList(ListNode* head)
{
    while (head != NULL)
    {
        printf("%s", head->value);
        head = head->next;
    }
}

void push(char* value)
{
    if (listHead == NULL)
    {
        listHead = (ListNode*)malloc(sizeof(ListNode));
        listHead->prev = NULL;
        listHead->next = NULL;
        listHead->value = value;
        return;
    }

    ListNode* it = listHead;
    while (it->next != NULL)
    {
        it = it->next;
    }
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = it;
    it->next = newNode;
}

void freeList()
{
    struct ListNode* tmp;

    while (listHead != NULL)
    {
        tmp = listHead;
        listHead = listHead->next;
        free(tmp);
    }
}

void* newThread(void* args)
{
    usleep(strlen(args) * 100000);

    pthread_mutex_lock(&mutex);
    push(args);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Please, input file name!\n");
        return 0;
    }
    FILE* text;
    if ((text = fopen(argv[1], "r")) == NULL)
    {
        printf("Error! File with name %s is not exist\n", argv[1]);
        return 0;
    }
    char str[64][100];
    int k = 0;
    while (fgets(str[k], 64, text) != NULL)
    {
        k++;
    }
    if (k > 100)
    {
        printf("A lot of strings!\n");
        return 0;
    }
    pthread_t* threads = malloc(sizeof(pthread_t) * k);
    int code;
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < k; i++)
    {
        code = pthread_create(&threads[i], NULL, newThread, (void*)str[i]);
        if (code != 0)
        {
            free(threads);
            print_error(code, argv);
        }
    }
    for (int i = 0; i < k; i++)
    {
        code = pthread_join(threads[i], NULL);
        if (code != 0)
        {
            free(threads);
            print_error(code, argv);
        }
    }
    printList(listHead);
    free(threads);
    freeList();
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
