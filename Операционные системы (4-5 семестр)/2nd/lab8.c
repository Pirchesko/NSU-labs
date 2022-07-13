#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define num_steps 200000000

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

void* calculate(void* param)
{
    struct threadArguments* args = (struct threadArguments*)param;
    double pi = 0.0;
    for (int i = args->threadNum; i < num_steps; i += args->threadCount)
    {
        pi += 1.0 / (i * 4.0 + 1.0);
        pi -= 1.0 / (i * 4.0 + 3.0);
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
    pthread_t threads[threadCount];
    struct threadArguments arguments[threadCount];
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
    pthread_exit(NULL);
}
