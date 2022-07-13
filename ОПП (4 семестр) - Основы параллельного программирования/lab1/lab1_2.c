#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

//----------Operations----------

// |x|=sqrt(sum(x_i^2))
double modul(double* x, int M)
{
    double sum = 0.0;
    for (int i = 0; i < M; i++)
    {
        sum += x[i] * x[i];
    }
    double allsum = 0.0;
    MPI_Allreduce(&sum, &allsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return sqrt(allsum);
}

// (x,y)=sum(x_i*y_i)
double skobki(double* x, double* y, int M)
{
    double res = 0.0;
    for (int i = 0; i < M; i++)
    {
        res += x[i] * y[i];
    }
    double allres = 0.0;
    MPI_Allreduce(&res, &allres, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return allres;
}

//multiplicate matrix to vector: result = x * y (N - size; x - matrix; result, y - vector)
void mul_M_v(double* result, double* x, double* y, int N, int M, int rank)
{
    double* res = (double*)calloc(M, sizeof(double));
    for (int i = 0; i < M; i++)
    {
        res[i] = 0.0;
    }
    double* Y = (double*)calloc(N, sizeof(double));
    MPI_Allgather(y, M, MPI_DOUBLE, Y, M, MPI_DOUBLE, MPI_COMM_WORLD);
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            res[i] += x[i * N + j] * Y[j];
        }
    }
    for (int i = 0; i < M; i++)
    {
        result[i] = res[i];
    }
    free(Y);
    free(res);
}

//multiplicate constant to vector: result = c * y (N - size; c - constant; result, y - vector)
void mul_c_v(double* result, double c, double* y, int M)
{
    for (int i = 0; i < M; i++)
    {
        result[i] = c * y[i];
    }
}

//substraction vector to vector: result = x - y (N - size; result, x, y - vector)
void sub(double* result, double* x, double* y, int M)
{
    for (int i = 0; i < M; i++)
    {
        result[i] = x[i] - y[i];
    }
}


//----------Setters/Getters/Print----------

void random_vector(double* x, int N, int M) 
{
    double* vector = (double*)calloc(N, sizeof(double));
    for (int i = 0; i < N; i++)
    {
        vector[i] = (double)(rand() % 18 - 9);
    }
    MPI_Scatter(vector, M, MPI_DOUBLE, x, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    free(vector);
}

void symmetrical_matrix(double* A, int N, int M)
{
    double* matrix = (double*)calloc(N * N, sizeof(double));
    for (int i = 0; i < N; i++)
    {
        matrix[i * N + i] = (double)(rand() % 18 - 9);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            matrix[i * N + j] = (double)(rand() % 18 - 9);
            matrix[j * N + i] = matrix[i * N + j];
        }
    }
    MPI_Scatter(matrix, M * N, MPI_DOUBLE, A, M * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    free(matrix);
}

void print_matrix(double* A, int N, int M, int rank, int size)
{
    for (int h = 0; h < size; h++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == h)
        {
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    printf("%2g ", A[i * N + j]);
                }
                printf("\n");
            }
        }
    }
}

void print_vector(double* V, int M, int rank, int size)
{
    if (rank == 0)
    {
        printf("(");
    }
    for (int h = 0; h < size; h++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == h)
        {
            for (int i = 0; i < M; i++)
            {
                printf("%g ", V[i]);
            }
        }
    }
    if (rank == 0)
    {
        printf(")\n");
    }
}


//----------Algorithm----------

void y_pow(double* y, double* A, double* x, double* b, int N, int M, int rank)
{
    double* Ax = (double*)calloc(M, sizeof(double));
    mul_M_v(Ax, A, x, N, M, rank);
    sub(y, Ax, b, M);
    free(Ax);
}

double t_pow(double* y, double* A, int N, int M, int rank)
{
    double* Ay = (double*)calloc(M, sizeof(double));
    mul_M_v(Ay, A, y, N, M, rank);
    double t = skobki(y, Ay, M) / skobki(Ay, Ay, M);
    free(Ay);
    return t;
}

void x_pow(double* y, double* A, double* x, double* b, int N, int M, int rank)
{
    y_pow(y, A, x, b, N, M, rank);
    double t = t_pow(y, A, N, M, rank);
    double* ty = (double*)calloc(M, sizeof(double));
    mul_c_v(ty, t, y, M);
    sub(x, x, ty, M);
    free(ty);
}

int test(double* y, double* b, double epsilon, int M)
{
    double t = modul(y, M) / modul(b, M);
    if (t < epsilon)
    {
        return 1; //остановка
    }
    else
    {
        return 0; //продолжай, пока не 1
    }
}

int main(int argc, char* argv[])
{
    //Фиксируем время начала
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    //MPI
    int size;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("Process %d of %d\n", rank, size);
    //Входные данные
    srand(1); //чтобы входные данные при каждом запуске были разные
    int N = 96;
    if (N % size == 0)
    {
        int M = N / size; //делим на ровные кусочки
        double epsilon = 0.0001;
        double* A = (double*)calloc(N * M, sizeof(double));
        double* x = (double*)calloc(M, sizeof(double));
        double* b = (double*)calloc(M, sizeof(double));
        double* y = (double*)calloc(M, sizeof(double)); // y=Ax-b
        //Создаём матрицу A и x, считаем b, а затем переписываем x (как раз далее будем этот вектор находить)
        symmetrical_matrix(A, N, M);
        random_vector(x, N, M);
        mul_M_v(b, A, x, N, M, rank);
        random_vector(x, N, M);
        y_pow(y, A, x, b, N, M, rank);
        //Начало алгоритма
        int i = 0; //счётчик итераций
        while (test(y, b, epsilon, M) == 0)
        {
            x_pow(y, A, x, b, N, M, rank);
            i++;
        }
        printf("Steps: %d\n", i);
        free(A);
        free(x);
        free(b);
        free(y);
    }
    else
    {
        printf("Error! Matrix size is not a multiple of the number of threads: size % threads != 0\n");
    }
    MPI_Finalize();
    //Фиксируем время конца
    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    printf("Time: %lf\n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
    return 0;
}