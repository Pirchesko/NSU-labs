/* 
Encoding ANSI (for example open in Visual Studio)
Так как доказательство 12-ой лабы было основано на том, что мы лишались корректной работы программы (lab12/lab12.c), то
мы должны обосновать, почему 14-ая будет в любом случае корректна или наоборот привести какой-то контраргумент к этому.
--------------------------------------------------
	parent						child
--------------------------------------------------
	sem_init(parent, 0, 1)
	sem_init(child, 0, 0)
	start child
--------------------------------------------------
	sem_wait(parent)			sem_wait(child)
	print						print
	sem_post(child)				sem_post(parent)
--------------------------------------------------
В лабе 14-ой мы использовали 2 счётчика-семафора.
Чтобы программа начала корректную свою работу, необходимо правильно инициализировать счётчки. 
Обязатнльно у одного должен быть 0, а у другого 1. Либо наоборот. С нас требуют чтоб первый выводился parent, значит ему остаим 1.
Если внимательно мысленно пронаблюдать как всё это работает, то можно прийти к выводу о том, что здесь никогда не будет такого,
чтобы дважды произошёл у кого-то sem_wait или sem_post, как это было с мутексами. 
У мутексов, как я считаю, главный косяк в том, что можно одним и тем же потоком зайти в критическую секцию несколько раз в цикле,
в то время как sem_wait (если его использовать с начальным счётчиком 1) неважно каким потоком - всё равно можно зайти ТОЛЬКО ОДИН РАЗ.
Сколько в счётчике осталось, столько раз можно зайти в ту секцию любым потоком и нужное количество раз (пока счётчик не стане равным 0)
Поэтому при правильной инициализации мы не сможем получить ни дедлока, ни рандомного вывода.
Чтобы получить дедлок необходимо оба семафора-счётчика проиницализировать нулями.
А чтобы получить неправильный вывод, можно проинициализировать счётчики единицами или более большими цифрами.
Т.е. надо специально закосячить. 
Итого: с 2-мя мутексами существовать стабильное решение - не может, с 2-мя семаформаи - может.
Далее код 14-ой лабы:
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define LINES_COUNT 10

sem_t parentSem;
sem_t childSem;

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
		sem_wait(&parentSem);
		printf("Parent\n");
		sem_post(&childSem);
	}
}

void* printChild(void* arg)
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		sem_wait(&childSem);
		printf("Child\n");
		sem_post(&parentSem);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t thread;

	sem_init(&parentSem, 0, 1);
	sem_init(&childSem, 0, 0);

	int error_code = pthread_create(&thread, NULL, printChild, NULL);
	if (error_code != 0)
	{
		print_error(error_code, argv);
	}
	printParent();

	sem_destroy(&parentSem);
	sem_destroy(&childSem);
	pthread_exit(NULL);
}
