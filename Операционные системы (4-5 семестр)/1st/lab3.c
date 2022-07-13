#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
	if (argc < 2)
	{
		perror("No input file! (no argument)!\n");
		return 0;
	}

	printf("Frist check ID:\n");
	printf("Real id user (uid) = %ld\n", getuid());
	printf("Effective id user (euid) = %ld\n", geteuid());
	
	FILE* file;
	file = fopen(argv[1], "r"); //https://www.opennet.ru/man.shtml?topic=fopen&category=3&russian=0
	if (file != NULL)
	{
		printf("Frist open file successed!\n");
		fclose(file);
	}
	else
	{
		perror("Frist open file failed!"); //https://www.opennet.ru/man.shtml?topic=perror&category=3&russian=0
		return 0;
	}
	
	seteuid(getuid()); //https://www.opennet.ru/man.shtml?topic=setuid&category=2&russian=0
	
	printf("Second check ID:\n");
	printf("Real id user (uid) = %ld\n", getuid());
	printf("Effective id user (euid) = %ld\n", geteuid());
	file = fopen(argv[1], "r");
	if (file != NULL)
	{
		printf("Second open file successed!\n");
		fclose(file);
	}
	else
	{
		perror("Second open file failed!");
		return 0;
	}

	return 0;
}