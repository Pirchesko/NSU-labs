#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* new_thread(void* param)
{
    for (int i = 0; i < 10; i++)
    {
        printf("Child\n");
    }
    //sleep(1);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    int error_code = pthread_create(&thread, NULL, new_thread, NULL);
    if (error_code != 0)
    {
        char buf[256];
        strerror_r(error_code, buf, sizeof buf);
        fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
        exit(1);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("Parent\n");
    }
    pthread_exit(NULL);
}
