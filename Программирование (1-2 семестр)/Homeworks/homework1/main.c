#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>

void print(int* mas, int N)
{
    for (int i = 0; i < N; i++)
    {
        printf("%d ", mas[i]);
    }
    printf("\n");
}

int NOD(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    else
    {
        return NOD(b, (a % b));
    }
}

int reverse(int* mas, int k, int N)
{
    int index, new_index, mas0;
    int cycle = NOD(k, N); //находим НОД чтоб пройти все элементы, которые остались после первого цикла
    for (int i = 0; i < cycle; i++)
    {
        mas0 = mas[i];
        index = i;
        int check = 1;
        while (check == 1)
        {
            new_index = index + k;
            new_index = new_index % N;
            if (new_index == i) //если пришло в исходную позицию, то выхоодим из цикла
            {
                check = 0;
                break;
            }
            mas[index] = mas[new_index];
            index = new_index;
            print(mas, N);
        }
        mas[index] = mas0;
    }
}

int main()
{
    system("chcp 1251");
    system("cls");
    
    int N;
    printf("Количество элементов: ");
    scanf("%d", &N);

    int k;
    printf("Сдвиг: ");
    scanf("%d", &k);

    //Автоматическое заполение массива
    int* mas = (int)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
    {
        mas[i] = i + 1;
    }

    printf("Введено: ");
    print(mas, N);

    reverse(mas, (N - k), N);

    printf("Ответ: ");
    print(mas, N);
}
//#include <stdio.h>
//
///*1 2 3 4 5 6 7
//6 7 1 2 3 4 5*/
//
//void reverse(int a[], int n, int k) {
//    if (k > n)
//        return;
//    int tmp = a[0], cur;
//    int ind = k;
//    for (int i = 0; i < n; ++i) {
//        cur = a[ind];
//        a[ind] = tmp;
//        tmp = cur;
//        ind = (ind + k) % n;
//    }
//}
//
//int main() {
//    int mas[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//    reverse(mas, 8, 6);
//    for (int i = 0; i < 8; ++i) {
//        printf("%d ", mas[i]);
//    }
//    return 0;
//}
