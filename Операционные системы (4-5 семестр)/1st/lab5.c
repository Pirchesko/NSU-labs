#include <stdio.h>
#include <sys/types.h> //open(), lseek()
#include <sys/stat.h> //open()
#include <fcntl.h> //open()
#include <unistd.h> //read(), lseek(), close()
#include <malloc.h>
//#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int file;
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

	while (1)
	{
		printf("String number: ");
		int strNumber;
		char buf[64];
		scanf("%s", &buf);
		strNumber = atoi(buf);
		if ((buf[0] != '0') && (strNumber == 0))
		{
			strNumber = -1;
		}
		//printf("%d", strNumber);
		if (strNumber == 0)
		{
			printf("Exiting...\n");
			break;
		}
		else if ((strNumber < 0) || (strNumber > strlenthNum - 1))
		{
			printf("Wrong string number, repeat.\n");
			continue;
		}
		lseek(file, strOffset[strNumber], SEEK_SET);
		char* str = (char*)calloc(strLenth[strNumber] + 1, sizeof(char));
		read(file, str, strLenth[strNumber]);
		printf("%s", str);
		free(str);
	}
	close(file); //http://www.opennet.ru/man.shtml?topic=close&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
	return 0;
}