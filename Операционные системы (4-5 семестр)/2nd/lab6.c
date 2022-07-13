#include <stdio.h>
#include <pthread.h>
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

void* newThread(void* param)
{
	usleep(strlen((char*)param) * 100000);
	printf("%s", (char*)param);
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
        printf("Error! File with name %s does not exist\n", argv[1]);
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
	free(threads);
	pthread_exit(NULL);
}