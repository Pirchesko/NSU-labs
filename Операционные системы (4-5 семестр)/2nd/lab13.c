#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define LINES_COUNT 10

pthread_cond_t condVar;
pthread_mutex_t mutex;
int parentCanPrint = 1;

void print_error(int error_code, char* argv[])
{
	char buf[256];
	strerror_r(error_code, buf, sizeof buf);
	fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
	exit(1);
}

void printParent()
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		pthread_mutex_lock(&mutex);
		if (parentCanPrint == 0)
		{
			pthread_cond_wait(&condVar, &mutex);
		}
		printf("Parent\n");
		parentCanPrint = 0;
		pthread_cond_signal(&condVar);
		pthread_mutex_unlock(&mutex);
	}
}

void* printChild(void* arg)
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		pthread_mutex_lock(&mutex);
		if (parentCanPrint == 1)
		{
			pthread_cond_wait(&condVar, &mutex);
		}
		printf("Child\n");
		parentCanPrint = 1;
		pthread_cond_signal(&condVar);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t thread;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condVar, NULL);

	int error_code = pthread_create(&thread, NULL, printChild, NULL);
	if (error_code != 0)
	{
		print_error(error_code, argv);
	}
	printParent();

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condVar);
	pthread_exit(NULL);
}

//https://learnc.info/c/pthreads_conditional_variables.html
//https://www.it-swarm.com.ru/ru/c/pochemu-funkcii-uslovnyh-peremennyh-pthreads-trebuyut-myuteksa/969974122/
//http://dmilvdv.narod.ru/Translate/ELSDD/elsdd_thread_synchronization.html