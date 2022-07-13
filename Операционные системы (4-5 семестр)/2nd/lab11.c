#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define LINES_COUNT 10
#define MUTEX_COUNT 3
#define PARENT_INDEX 0
#define CHILD_INDEX 1

pthread_mutex_t mutex[MUTEX_COUNT];
char* strParent = "Parent\n";
char* strChild = "Child\n";

void print_error(int error_code, char* argv[])
{
	char buf[256];
	strerror_r(error_code, buf, sizeof buf);
	fprintf(stderr, "%s: creating thread: %s\n", argv[0], buf);
	exit(1);
}

int init_mutexes()
{
	for (int i = 0; i < MUTEX_COUNT; i++)
	{
		int error_code = pthread_mutex_init(&mutex[i], NULL);
		if (error_code != 0)
		{
			return error_code;
		}
	}
	return 0;
}

int delete_mutexes()
{
	for (int i = 0; i < MUTEX_COUNT; i++)
	{
		int error_code = pthread_mutex_destroy(&mutex[i]);
		if (error_code != 0)
		{
			return error_code;
		}
	}
	return 0;
}

int lock(int index)
{
	return pthread_mutex_lock(&mutex[index]);
}

int unlock(int index)
{
	return pthread_mutex_unlock(&mutex[index]);
}

/*
----------------------------
	parent		child
----------------------------
	lock(1)		lock(2)
	start child
	sleep
----------------------------
	lock(0)		lock(1)
	print		print
	unlock(1)	unlock(2)

	lock(2)		lock(0)
	print		print
	unlock(0)	unlock(1)

	lock(1)		lock(2)
	print		print
	unlock(2)	unlock(0)
----------------------------
*/

int print_lines(char* line, int count, int thread_index)
{
	int error_code;
	for (int i = 0; i < count; i++)
	{
		error_code = lock((2 * i + thread_index) % MUTEX_COUNT); //PARENT_INDEX = 0, CHILD_INDEX = 1
		if (error_code != 0)
		{
			return error_code;
		}
		printf("%s", line);
		error_code = unlock((2 * i + 1 + thread_index) % MUTEX_COUNT);
		if (error_code != 0)
		{
			return error_code;
		}
	}
	return 0;
}

void* thread_control(void* arg)
{
	lock(2);
	int error_code = print_lines(strChild, LINES_COUNT, CHILD_INDEX);
	if (error_code != 0)
	{
		errno = error_code;
		perror("Error in child thread");
		pthread_exit(NULL);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int error_code;
	pthread_t thread;
	init_mutexes();
	lock(1);
	error_code = pthread_create(&thread, NULL, thread_control, NULL);
	if (error_code != 0)
	{
		print_error(error_code, argv);
	}
	error_code = pirnt_lines(strParent, LINES_COUNT, PARENT_INDEX);
	if (error_code != 0)
	{
		print_error(error_code, argv);
	}
	delete_mutexes();
	pthread_exit(NULL);
}

//https://learnc.info/c/pthreads_semaphores.html
