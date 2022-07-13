#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define LINES_COUNT 10

sem_t* parentSem;
sem_t* childSem;

void printParent()
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		sem_wait(parentSem);
		printf("Parent\n");
		sem_post(childSem);
	}
}

void printChild()
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		sem_wait(childSem);
		printf("Child\n");
		sem_post(parentSem);
	}
	return;
}

int main()
{
	parentSem = sem_open("/parentSem", O_CREAT, O_RDWR, 1);
	if (parentSem == SEM_FAILED)
	{
		printf("Error in open parentSem!\n");
		return 0;
	}
	childSem = sem_open("/childSem", O_CREAT, O_RDWR, 0);
	if (childSem == SEM_FAILED)
	{
		printf("Error in open childSem!\n");
		return 0;
	}
	int who = fork();
	if (who == 0)
	{
		printChild();
		int close = sem_unlink("/childSem");
		if (close != 0)
		{
			printf("Error in unlink childSem!\n");
			return 0;
		}
		sem_close(childSem);
	}
	else
	{
		printParent();
		int close = sem_unlink("/parentSem");
		if (close != 0)
		{
			printf("Error in unlink parentSem!\n");
			return 0;
		}
		sem_close(parentSem);
	}
	return 0;
}

//https://www.opennet.ru/man.shtml?topic=sem_open&category=3&russian=4
//https://www.opennet.ru/man.shtml?topic=sem_close&russian=4&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
//https://www.opennet.ru/man.shtml?topic=sem_unlink&russian=4&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
