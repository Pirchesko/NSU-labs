#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* new_thread(void* param)
{
    for (char** str = (char**)param; *str != NULL; str++)
    {
        printf("%s\n", *str);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t threads[4];
    char* text[4][4] = { {"11", "12", "13", NULL},{"21", "22", "23", NULL},{"31", "32", "33", NULL},{"41", "42", "43", NULL}};
    int error_code;
    for (int i = 0; i < 4; i++)
    {
        error_code = pthread_create(&threads[i], NULL, new_thread, &text[i]);
        if (error_code != 0)
        {
            char buf[256];
            strerror_r(error_code, buf, sizeof buf);
            fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
            exit(1);
        }
    }
    pthread_exit(NULL);
}
