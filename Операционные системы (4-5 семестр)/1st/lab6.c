#include <stdio.h>
#include <sys/types.h> //open(), lseek()
#include <sys/stat.h> //open()
#include <fcntl.h> //open()
#include <unistd.h> //read(), lseek(), close(), sleep()
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int print(int* check, int strNumber, int strlenthNum, int file, int* strOffset, int* strLenth)
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
	lseek(file, strOffset[strNumber], SEEK_SET);
	char* str = (char*)calloc(strLenth[strNumber] + 1, sizeof(char));
	read(file, str, strLenth[strNumber]);
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
	int strLenth[64];
	int strOffset[64];
	int strlenthNum = 1;
	int strOffsetNum = 0;
	strOffset[0] = 0;
	strOffset[1] = 0;
	while (read(file, &c, 1)) //http://www.opennet.ru/man.shtml?topic=read&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
	{
		strOffsetNum++;
		if (c == '\n')
		{
			strLenth[strlenthNum] = strOffsetNum;
			strlenthNum++;
			strOffset[strlenthNum] = lseek(file, 0, SEEK_CUR); //http://www.opennet.ru/man.shtml?topic=lseek&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
			strOffsetNum = 0;
		}
	}

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
				for (int i = 0; i < strlenthNum; i++)
				{
					lseek(file, strOffset[i], SEEK_SET);
					char* str_ = (char*)calloc(strLenth[i] + 1, sizeof(char));
					read(file, str_, strLenth[i]);
					printf("%s", str_);
					free(str_);
				}
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
				if (print(&check, strNumber, strlenthNum, file, strOffset, strLenth) == 0)
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
			if (print(&check, strNumber, strlenthNum, file, strOffset, strLenth) == 0)
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