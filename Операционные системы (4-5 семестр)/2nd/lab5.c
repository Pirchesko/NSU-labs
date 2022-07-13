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

void stop_spam(void* param)
{
    printf("\nStopping spam!\n");
}

void* new_thread(void* param)
{
    pthread_cleanup_push(stop_spam, NULL);
    while (1)
    {
        printf("Spam! ");
        pthread_testcancel();
    }
    pthread_cleanup_pop(1);
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
    if (error_code != 0)
    {
        print_error(error_code, argv);
    }
    pthread_exit(NULL);
}

//588 строка pthread.h