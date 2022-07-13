#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

//----------Setters/Getters/Print----------

void random_vector(double* x, int N)
{
    for (int i = 0; i < N; i++)
    {
        x[i] = (double)(rand() % 18 - 9);
    }
}

void symmetrical_matrix(double* A, int N)
{
    for (int i = 0; i < N; i++)
    {
        A[i * N + i] = (double)(rand() % 18 - 9);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            A[i * N + j] = (double)(rand() % 18 - 9);
            A[j * N + i] = A[i * N + j];
        }
    }
}

void print_matrix(double* A, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%2g ", A[i * N + j]);
        }
        printf("\n");
    }
}

void print_vector(double* V, int N)
{
    printf("(");
    for (int i = 0; i < N; i++)
    {
        printf("%g ", V[i]);
    }
    printf(")\n");
}

int main()
{
    //Входные данные
    srand(278); //чтобы входные данные при каждом запуске были разные
    int N = 1024;
    double epsilon = 0.003;
    double* A = (double*)malloc(N * N * sizeof(double));
    double* x = (double*)malloc(N * sizeof(double));
    double* b = (double*)malloc(N * sizeof(double));
    printf("%d; ", omp_get_max_threads());
    //Создаём матрицу A и x, считаем b, а затем переписываем x (как раз далее будем этот вектор находить)
    symmetrical_matrix(A, N);
    random_vector(x, N);
    //print_vector(x, 10);
    for (int i = 0; i < N; i++)
    {
        b[i] = 0;
        for (int j = 0; j < N; j++)
        {
            b[i] += A[i * N + j] * x[j];
        }
    }
    random_vector(x, N);
    //Начало алгоритма
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;
    double sum4 = 0.0;
    double t;
    int check = 1;
    double* Ax = (double*)malloc(N * sizeof(double));
    double* Ay = (double*)malloc(N * sizeof(double));
    double* ty = (double*)malloc(N * sizeof(double));
    double* y = (double*)malloc(N * sizeof(double));
    //Фиксируем время начала
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int k = 1; //счётчик итераций
    #pragma omp parallel
    {
        while (check == 1)
        {           
            #pragma omp single
            {
                sum1 = 0.0;
                sum2 = 0.0;
                sum3 = 0.0;
                sum4 = 0.0;
            }
            #pragma omp for reduction(+: sum1, sum2) 
            for (int i = 0; i < N; i++)
            {
                Ax[i] = 0;
                for (int j = 0; j < N; j++)
                {
                    Ax[i] += A[i * N + j] * x[j];
                }
                y[i] = Ax[i] - b[i];
                sum1 += y[i] * y[i];
                sum2 += b[i] * b[i];
            }
            #pragma omp single
            {
                if ((sqrt(sum1) / sqrt(sum2)) < epsilon)
                {
                    check = 0;
                }
            }
            if (check == 1)
            {
                #pragma omp for reduction(+: sum3, sum4)
                for (int i = 0; i < N; i++)
                {
                    Ay[i] = 0;
                    for (int j = 0; j < N; j++)
                    {
                        Ay[i] += A[i * N + j] * y[j];
                    }
                    sum3 += y[i] * Ay[i];
                    sum4 += Ay[i] * Ay[i];
                }
                #pragma omp single
                {
                    t = sum3 / sum4;
                }
                #pragma omp for 
                for (int i = 0; i < N; i++)
                {
                    ty[i] = t * y[i];
                    x[i] = x[i] - ty[i];
                }
                #pragma omp single
                {
                    k++;
                }
            }
        }
    }
    //Фиксируем время конца
    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    printf("%d; ", k);
    printf("%lf; \n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
    //print_vector(x, 10);
    free(A);
    free(x);
    free(b);
    free(Ax);
    free(Ay);
    free(ty);
    free(y);
    return 0;
}