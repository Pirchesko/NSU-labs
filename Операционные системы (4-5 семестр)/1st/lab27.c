#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	FILE* fileIn;
	FILE* fileOut;

	if ((fileIn = fopen(argv[1], "r")) == NULL)
	{
		perror("Error in open file");
		return 0;
	}

	int size = strlen(argv[1]);
	char str[64] = { "wc -l " };
	strncat(str, argv[1], size);
	//printf("\n");

	if ((fileOut = popen(str, "w")) == NULL)
	{
		perror("Error in starting program wc -l");
		return 0;
	}

	fclose(fileIn);
	pclose(fileOut);
	return 0;
}