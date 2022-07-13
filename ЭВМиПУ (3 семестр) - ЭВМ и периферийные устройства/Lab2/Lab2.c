#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 120000

int main()
{
    //Объявленные переменные
    int massive[N]; //массив который будем сортировать
    int start; //фиксирование времени начала сортировки
    int finish; //фиксирование времени после сортировки

    //Сохдаём массив с рандомными значениями в ячейках
    srand(1);
    for (int i = 0; i < N; i++)
    {
        massive[i] = rand();
    }

    printf("Before: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d; ", massive[i]);
    }
    printf("...\n");

    //Сортируем пузырьком
    printf("Start sort...\n");
    start = clock();
    for (int i = 0; i < N; i++)
    {
        for (int j = i; j < N; j++)
        {
            if (massive[i] < massive[j])
            {
                int temp = massive[i];
                massive[i] = massive[j];
                massive[j] = temp;
            }
        }
    }
    finish = clock();
    printf("Finish!\n");

    //Выводим время
    printf("Time = %d\n", (finish - start));

    printf("After: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d; ", massive[i]);
    }
    printf("...\n\n");

    return 0;
}
