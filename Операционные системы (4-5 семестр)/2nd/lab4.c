#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void print_error(int error_code, char* argv[])
{
    char buf[256];
    strerror_r(error_code, buf, sizeof buf);
    fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
    exit(1);
}

void* new_thread(void* param)
{
    while (1)
    {
        printf("Spam! ");
    }
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    int error_code = pthread_create(&thread, NULL, new_thread, NULL);
    if (error_code != 0)
    {
        print_error(error_code, argv);
    }
    sleep(2);
    error_code = pthread_cancel(thread);
    printf("\n");
    if (error_code != 0)
    {
        print_error(error_code, argv);
    }
    pthread_exit(NULL);
}
