#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct list
{
	char* word;
	struct list* next;
} list;

list* addWord(list* words, char* word)
{
	words->word = (char*)calloc(strlen(word) + 1, sizeof(char));
	strcpy(words->word, word);
	list* buffer = (list*)malloc(sizeof(list));
	buffer->word = NULL;
	buffer->next = NULL;
	words->next = buffer;
	return buffer;
}

list* clear(list* words)
{
	if (words == NULL)
	{
		return NULL;
	}
	else
	{
		list* buffer;
		buffer = words->next;
		free(words->word);
		free(words);
		return buffer;
	}
}

int main()
{
	char word[64];
	list* begin = (list*)malloc(sizeof(list));
	begin->word = NULL;
	begin->next = NULL;
	list* words = begin;

	while ((fgets(word, sizeof(word), stdin) != EOF) && (word[0] != '.')) //https://www.opennet.ru/man.shtml?topic=fgets&category=3&russian=0
	{
		words = addWord(words, word);
	}

	words = begin;

	while (words->word != NULL)
	{
		printf("%s", words->word);
		words = words->next;
	}

	while (begin != NULL)
	{
		begin = clear(begin);
	}

	return 0;
}