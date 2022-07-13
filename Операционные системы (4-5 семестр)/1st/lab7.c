#include <stdio.h>
#include <sys/types.h> //open(), lseek()
#include <sys/stat.h> //open()
#include <fcntl.h> //open()
#include <unistd.h> //read(), lseek(), close(), sleep()
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h> //mmap()

int print(int* check, int strNumber, int strlenthNum, int file, char* strOffset[], int* strLenth, char* p)
{
	if (strNumber == 0)
	{
		printf("Exiting...\n");
		return 0;
	}
	else if ((strNumber < 0) || (strNumber > strlenthNum - 1))
	{
		printf("Wrong string number, repeat.\n");
		*check = 0;
		return 1;
	}
	char* str = (char*)calloc(strLenth[strNumber] + 1, sizeof(char));
	memcpy(str, strOffset[strNumber], strLenth[strNumber]);
	printf("%s", str);
	free(str);
	*check = 0;
}

int main(int argc, char* argv[])
{
	int file;
	int terminal;
	if (argc < 2)
	{
		perror("Check arguments!");
		return 0;
	}
	file = open(argv[1], O_RDONLY); //http://www.opennet.ru/man.shtml?topic=open
	if (file == -1)
	{
		perror("Error in open file!");
		return 0;
	}
	terminal = open("/dev/tty", O_RDONLY | O_NDELAY);

	char c;
	int size;
	struct stat file_stat;
	if (stat(argv[1], &file_stat) == 0)
	{
		size = file_stat.st_size;
	}
	else
	{
		printf("Error in function stat(), using lseek()\n");
		size = lseek(file, 0, SEEK_END);
	}
	char* p = mmap(0, size, PROT_READ, MAP_SHARED, file, 0); //http://www.opennet.ru/man.shtml?topic=mmap&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+manfor
	int strLenth[64];
	static char* strOffset[64];
	int strlenthNum = 1;
	int strOffsetNum = 0;
	strOffset[0] = 0;
	strOffset[1] = p;
	for (int i = 0; i < size; i++)
	{
		strOffsetNum++;
		if (*(p + i) == '\n')
		{
			strLenth[strlenthNum] = strOffsetNum;
			strlenthNum++;
			strOffset[strlenthNum] = i + p + 1; //http://www.opennet.ru/man.shtml?topic=lseek&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
			strOffsetNum = 0;
		}
	}
	strOffset[strlenthNum] = 0;

	char buf[64];
	int check = 1;
	while (1)
	{
		if (check == 1)
		{
			printf("You have 5 seconds to put string number:\n");
			sleep(5);
			int reading = read(terminal, buf, 64);
			if ((reading == 0) && (check == 1))
			{
				printf("\nAll strings:\n");
				printf("%s", strOffset[1]);
				check = 0;
				break;
			}
			else
			{
				int strNumber;
				strNumber = atoi(buf);
				if ((buf[0] != '0') && (strNumber == 0))
				{
					strNumber = -1;
				}
				if (print(&check, strNumber, strlenthNum, file, strOffset, strLenth, p) == 0)
				{
					return 0;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			printf("Input string number: ");
			int strNumber;
			scanf("%s", &buf);
			strNumber = atoi(buf);
			if ((buf[0] != '0') && (strNumber == 0))
			{
				strNumber = -1;
			}
			if (print(&check, strNumber, strlenthNum, file, strOffset, strLenth, p) == 0)
			{
				return 0;
			}
			else
			{
				continue;
			}
		}
	}
	close(terminal);
	close(file); //http://www.opennet.ru/man.shtml?topic=close&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
	return 0;
}