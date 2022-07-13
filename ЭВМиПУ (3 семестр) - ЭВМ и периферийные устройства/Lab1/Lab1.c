#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 74250

int main()
{
    //ќбъ€вленные переменные
    int massive[N]; //массив который будем сортировать
    int start; //фиксирование времени начала сортировки
    int finish; //фиксирование времени после сортировки

    //—охдаЄм массив с рандомными значени€ми в €чейках
    srand(1);
    for (int i = 0; i < N; i++)
    {
        massive[i] = rand();
    }

    //—ортируем пузырьком
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

    //¬ыводим врем€
    printf("Time = %d\n", (finish - start));
    return 0;
}
