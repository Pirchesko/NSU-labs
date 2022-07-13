#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		perror("No arguments!");
		return 0;
	}
	int child;
	int parent;
	child = fork();
	if (child == 0)
	{
		sleep(3);
		execl("/bin/cat", "cat", argv[1], (char*)0); //int execl(const char *path, const char *arg, ...); должно заканчиваться на NULL
		//printf("Child: pid=%ld ppid=%ld\n", getpid(), getppid());
	}
	else
	{
		printf("Parent should print a text!\n");
		int status;
		parent = wait(&status);
		printf("Parent dozhdalsja!\n");
		//printf("Parent: pid=%ld ppid=%ld\n", getpid(), getppid());
		return 0;
	}
	
	return 0;
}

//./lab9.exe text.txt