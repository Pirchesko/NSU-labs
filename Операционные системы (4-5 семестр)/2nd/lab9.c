#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

pthread_barrier_t barrier;
int doIt = 1;
int sigExit = 0;

struct threadArguments
{
    int threadCount;
    int threadNum;
    double partSum;
};

void print_error(int error_code, char* argv[])
{
    char buf[256];
    strerror_r(error_code, buf, sizeof buf);
    fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
    exit(1);
}

void checker(int sig)
{
    if (sig == SIGINT)
    {
        sigExit = 1;
        printf("\nStopped!\n");
    }
}

void checkStop()
{
    if (sigExit == 1)
    {
        doIt = 0;
    }
}

void* calculate(void* param)
{
    struct threadArguments* args = (struct threadArguments*)param;
    double pi = 0.0;
    int i = args->threadNum;
    int count = 0;
    while (doIt == 1)
    {
        while (count < 1000000)
        {
            pi += 1.0 / (i * 4.0 + 1.0);
            pi -= 1.0 / (i * 4.0 + 3.0);
            i += args->threadCount;
            count++;
        }
        count = 0;
        pthread_barrier_wait(&barrier);
        checkStop();
        pthread_barrier_wait(&barrier);
    }
    args->partSum = pi;
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Please, input count of threads!\n");
        return 0;
    }
    int threadCount = -1;
    threadCount = atoi(argv[1]);
    if (threadCount < 1)
    {
        printf("Please, input positive count of threads!\n");
        return 0;
    }
    signal(SIGINT, checker);
    pthread_t threads[threadCount];
    struct threadArguments arguments[threadCount];
    pthread_barrier_init(&barrier, NULL, threadCount);
    int code;
    for (int i = 0; i < threadCount; i++)
    {
        arguments[i].threadCount = threadCount;
        arguments[i].threadNum = i;
        code = pthread_create(&threads[i], NULL, calculate, &arguments[i]);
        if (code != 0)
        {
            print_error(code, argv);
        }
    }
    double pi = 0.0;
    for (int i = 0; i < threadCount; i++)
    {
        code = pthread_join(threads[i], NULL);
        if (code != 0)
        {
            print_error(code, argv);
        }
        pi += arguments[i].partSum;
    }
    pi *= 4.0;
    printf("pi done - %.15g \n", pi);
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
}

//https://habr.com/ru/post/141206/
//сделать с баррьером