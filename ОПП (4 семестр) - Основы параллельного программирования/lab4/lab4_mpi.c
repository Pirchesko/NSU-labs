#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "mpi.h"

//Эта штука просто меняет предыдущий массив с текущим и наоборот (F[m] F[m+1 и наоборот])
void swapping(int* swap1, int* swap2)
{
    int temp = *swap1;
    *swap1 = *swap2;
    *swap2 = temp;
}

//Функция fi(x,y,z)
double fi(int i, int j, int k, double h_i, double h_j, double h_k, double I_0, double J_0, double K_0)
{
    double x = I_0 + i * h_i;
    double y = J_0 + j * h_j;
    double z = K_0 + k * h_k;
    return (double)(x * x + y * y + z * z);
}

//Функция ro(x,y,z)
double ro(int a, int i, int j, int k, double h_i, double h_j, double h_k, double I_0, double J_0, double K_0)
{
    return (6 - a * fi(i, j, k, h_i, h_j, h_k, I_0, J_0, K_0));
}

//Оценка точности полученного решения
double accuracy(double** F, int swap, int block, int rank, int a, int N_i, int N_j, int N_k, double h_i, double h_j, double h_k, double I_0, double J_0, double K_0)
{
    double max = (double)INT_MIN;
    double c;
    for (int i = 1; i < block - 1; i++)
    {
        int index_I = i * N_j * N_k;
        int J_K = N_j * N_k;
        for (int j = 1; j < N_j - 1; j++)
        {
            int index_J = j * N_k;
            for (int k = 1; k < N_k - 1; k++)
            {
                c = fabs(F[swap][index_I + index_J + k] - fi(i + (block * rank), j, k, h_i, h_j, h_k, I_0, J_0, K_0));
                if (max < c)
                {
                    max = c;
                }
            }
        }
    }
    return max;
}

//Проверка на критерий завершённости
void check(double** F, int swap1, int swap2, int* complete, double e, int index_I, int index_J, int k)
{
    if (fabs(F[swap2][index_I + index_J + k] - F[swap1][index_I + index_J + k]) < e)
    {
        *complete = 1;
    }
}

//Итерация - вычисление большой формулы (по краям)
void iteration_edge(double** F, double* up, double* down, int block, int rank, int size, int swap1, int swap2, int* complete, double e, int N_i, int N_j, int N_k, double coefficient, int a, double h_i, double h_j, double h_k, double h_i_2, double h_j_2, double h_k_2, double I_0, double J_0, double K_0)
{
    double F_i, F_j, F_k;
    for (int j = 1; j < N_j - 1; j++)
    {
        for (int k = 1; k < N_k - 1; k++)
        {
            if (rank != 0)
            {
                int i = 0;
                int index_I = i * N_j * N_k;
                int J_K = N_j * N_k;
                int index_J = j * N_k;
                int I = i + block * rank;
                F_i = (F[swap1][(index_I + index_J + k) + J_K] + down[/*index_I*/ + index_J + k]) / h_i_2;
                F_j = (F[swap1][(index_I + index_J + k) + N_k] + F[swap1][(index_I + index_J + k) - N_k]) / h_j_2;
                F_k = (F[swap1][(index_I + index_J + k) + 1] + F[swap1][(index_I + index_J + k) - 1]) / h_k_2;
                F[swap2][index_I + index_J + k] = coefficient * (F_i + F_j + F_k - ro(a, I, j, k, h_i, h_j, h_k, I_0, J_0, K_0));
                check(F, swap1, swap2, complete, e, index_I, index_J, k);
            }
            if (rank != size - 1)
            {
                int i = block - 1;
                int index_I = i * N_j * N_k;
                int J_K = N_j * N_k;
                int I = i + block * rank;
                int index_J = j * N_k;
                F_i = (up[/*index_I*/ + index_J + k] + F[swap1][(index_I + index_J + k) - J_K]) / h_i_2;
                F_j = (F[swap1][(index_I + index_J + k) + N_k] + F[swap1][(index_I + index_J + k) - N_k]) / h_j_2;
                F_k = (F[swap1][(index_I + index_J + k) + 1] + F[swap1][(index_I + index_J + k) - 1]) / h_k_2;
                F[swap2][index_I + index_J + k] = coefficient * (F_i + F_j + F_k - ro(a, I, j, k, h_i, h_j, h_k, I_0, J_0, K_0));
                check(F, swap1, swap2, complete, e, index_I, index_J, k);
            }
        }
    }
}

//Итерация - вычисление большой формулы (внутри)
void iteration_in(double** F, int block, int rank, int swap1, int swap2, int* complete, double e, int N_i, int N_j, int N_k, double coefficient, int a, double h_i, double h_j, double h_k, double h_i_2, double h_j_2, double h_k_2, double I_0, double J_0, double K_0)
{
    double F_i, F_j, F_k;
    for (int i = 1; i < block - 1; i++) //всего узлов N_i, т.е. с 0 до N_i-1 => считаем с 1 до N_i-2
    {
        int index_I = i * N_j * N_k;
        int J_K = N_j * N_k;
        int I = i + block * rank;
        for (int j = 1; j < N_j - 1; j++)
        {
            int index_J = j * N_k;
            for (int k = 1; k < N_k - 1; k++)
            {
                F_i = (F[swap1][(index_I + index_J + k) + J_K] + F[swap1][(index_I + index_J + k) - J_K]) / h_i_2;
                F_j = (F[swap1][(index_I + index_J + k) + N_k] + F[swap1][(index_I + index_J + k) - N_k]) / h_j_2;
                F_k = (F[swap1][(index_I + index_J + k) + 1] + F[swap1][(index_I + index_J + k) - 1]) / h_k_2;
                F[swap2][index_I + index_J + k] = coefficient * (F_i + F_j + F_k - ro(a, I, j, k, h_i, h_j, h_k, I_0, J_0, K_0));
                //printf("F[%d][%d][%d] = %lf\t;%d\n", I, j, k, F[swap2][index_I + index_J + k], (rank * block * N_j * N_k) + index_I + index_J + k);
                //printf("F[%d][%d][%d] = %lf\t;%d\n", (rank * block) + I, j, k, F[0][index_I + index_J + k], (rank * block * N_j * N_k) + index_I + index_J + k);
                check(F, swap1, swap2, complete, e, index_I, index_J, k);
            }
        }
    }
}

//Инициализируем массив-функцию F - задаём краевые условия
void init_F(double** F, int block, int rank, int size, int N_i, int N_j, int N_k, double h_i, double h_j, double h_k, double I_0, double J_0, double K_0)
{
    memset(F[0], 0.0, block * N_j * N_k * sizeof(double));
    memset(F[1], 0.0, block * N_j * N_k * sizeof(double));
    if (rank == 0)
    {
        for (int j = 1; j < N_j - 1; j++) //заполняем самый низ и вверх
        {
            for (int k = 1; k < N_k - 1; k++)
            {
                F[0][j * N_k + k] = fi(0, j, k, h_i, h_j, h_k, I_0, J_0, K_0); //для самого низа
                F[1][j * N_k + k] = fi(0, j, k, h_i, h_j, h_k, I_0, J_0, K_0);
                //printf("F[%d][%d][%d] = %lf\t;%d\n", 0, j, k, F[0][j * N_k + k], j * N_k + k);
            }
        }
    }
    if (rank == size - 1)
    {
        for (int j = 1; j < N_j - 1; j++) //заполняем самый низ и вверх
        {
            int index = (block - 1) * N_j * N_k; //для самого верха
            for (int k = 1; k < N_k - 1; k++)
            {
                F[0][index + j * N_k + k] = fi(N_i - 1, j, k, h_i, h_j, h_k, I_0, J_0, K_0);
                F[1][index + j * N_k + k] = fi(N_i - 1, j, k, h_i, h_j, h_k, I_0, J_0, K_0);
                //printf("F[%d][%d][%d] = %lf\t;%d\n", (rank * block) + (block - 1), j, k, F[0][index + j * N_k + k], (rank * block * N_j * N_k) + index + j * N_k + k);
            }
        }
    }
    for (int i = 0; i < block; i++) //заполняем перед и зад
    {
        int index = i * N_j * N_k; //для фронтальной части
        int index_ = index + N_k * (N_j - 1); //для задней части
        for (int k = 0; k < N_k; k++)
        {
            F[0][index + k] = fi(i + block * rank, 0, k, h_i, h_j, h_k, I_0, J_0, K_0);
            F[1][index + k] = fi(i + block * rank, 0, k, h_i, h_j, h_k, I_0, J_0, K_0);
            F[0][index_ + k] = fi(i + block * rank, N_j - 1, k, h_i, h_j, h_k, I_0, J_0, K_0);
            F[1][index_ + k] = fi(i + block * rank, N_j - 1, k, h_i, h_j, h_k, I_0, J_0, K_0);
            //printf("F[%d][%d][%d] = %lf\t;%d\n", i + (rank * block), 0, k, F[0][index + k], (rank * block * N_j * N_k) + index + k);
            //printf("F[%d][%d][%d] = %lf\t;%d\n", i + (rank * block), N_j - 1, k, F[0][index_ + k], (rank * block * N_j * N_k) + index_ + k);
        }
    }
    for (int i = 0; i < block; i++) //заполняем право и лево
    {
        int index = i * N_j * N_k; //для правой части (0)
        int index_ = index + N_k - 1; //для левой части (N_k)
        for (int j = 1; j < N_j - 1; j++)
        {
            F[0][index + j * N_k] = fi(i + block * rank, j, 0, h_i, h_j, h_k, I_0, J_0, K_0);
            F[1][index + j * N_k] = fi(i + block * rank, j, 0, h_i, h_j, h_k, I_0, J_0, K_0);
            F[0][index_ + j * N_k] = fi(i + block * rank, j, N_k - 1, h_i, h_j, h_k, I_0, J_0, K_0);
            F[1][index_ + j * N_k] = fi(i + block * rank, j, N_k - 1, h_i, h_j, h_k, I_0, J_0, K_0);
            //printf("F[%d][%d][%d] = %lf\t;%d\n", i + (rank * block), j, 0, F[0][index + j * N_k], (rank * block * N_j * N_k) + index + j * N_k);
            //printf("F[%d][%d][%d] = %lf\t;%d\n", i + (rank * block), j, N_k - 1, F[0][index_ + j * N_k], (rank * block * N_j * N_k) + index_ + j * N_k);
        }
    }
}

//Здесь вносятся входные данные и инициализируются массивы (0 - успех, 1 - неудача)
int memory_init(char** argv, int* N_i, int* N_j, int* N_k, double* e, int* a, double* I_0, double* J_0, double* K_0, double* D_i, double* D_j, double* D_k, double* h_i, double* h_j, double* h_k, double* h_i_2, double* h_j_2, double* h_k_2, double* coefficient, double*** F, double** up, double** down, int* swap1, int* swap2, int* complete, int* complete_all, int* count, int size, int* block)
{
    *N_i = 256;
    *N_j = 256;
    *N_k = 256;
    if ((argv[1] != 0) && (argv[2] != 0) && (argv[3] != 0))
    {
        *N_i = atoi(argv[1]);
        *N_j = atoi(argv[2]);
        *N_k = atoi(argv[3]);
    }
    if (*N_i % size != 0)
    {
        printf("Error! Use N_i only multiply of 16!\n");
        return 1;
    }
    *block = *N_i / size;
    if (*block < 3)
    {
        printf("Error! Use N_i more than now if you want a lot of threads!\n");
        return 1;
    }
    *e = 100000000;
    if (argv[4] != 0)
    {
        *e = atoi(argv[4]);
    }
    *e = 1 / *e;
    *a = 100000;
    *I_0 = -1.0;
    *J_0 = -1.0;
    *K_0 = -1.0;
    *D_i = 2.0;
    *D_j = 2.0;
    *D_k = 2.0;
    *h_i = *D_i / (double)(*N_i - 1);
    *h_j = *D_j / (double)(*N_j - 1);
    *h_k = *D_k / (double)(*N_k - 1);
    *h_i_2 = (*h_i) * (*h_i);
    *h_j_2 = (*h_j) * (*h_j);
    *h_k_2 = (*h_k) * (*h_k);
    *coefficient = 1.0 / (2.0 / (*h_i_2) + 2.0 / (*h_j_2) + 2.0 / (*h_k_2) + (*a));
    *F = (double**)malloc(2 * sizeof(double*));
    (*F)[0] = (double*)malloc((*block) * (*N_j) * (*N_k) * sizeof(double));
    (*F)[1] = (double*)malloc((*block) * (*N_j) * (*N_k) * sizeof(double));
    *up = (double*)malloc((*N_j) * (*N_k) * sizeof(double));
    *down = (double*)malloc((*N_j) * (*N_k) * sizeof(double));
    *swap1 = 0;
    *swap2 = 1;
    *complete = 0;
    *complete_all = 0;
    *count = 0;
    return 0;
}

int main(int argc, char** argv)
{
    //Необходимые входные данные и их описание
    int N_i, N_j, N_k; //размеры узлов
    double e; //порог сходимости (указывается обратной величниной, для удобства)
    int a; //параметр уравнения
    double I_0, J_0, K_0; //начальные координаты
    double D_i, D_j, D_k; //размеры моделирования
    double h_i, h_j, h_k; //шаги сетки
    double h_i_2, h_j_2, h_k_2; //шаги сетки в квадрате
    double coefficient; //посчитаем его, чтобы больше не пересчитывать
    double** F = NULL; //тут будет результат итеранционного алгортима (текущая и предыдущие и будут по очереди меняться)
    int swap1, swap2; //переменные которые отвечают за очередность в F[0] и F[1]
    double* up = NULL, *down = NULL; //это буфера верхних и нижних значений
    int complete, complete_all; //проверка на завершение работы алгоритма (критерий завершёности)
    int count; //просто счётчик итераций (для интереса)
    int block; //высота каждого I-ого блока

    //Начало алгоритма
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request send_down_to_up; //отвечает за доставку исходной НИЖНЕЙ части в качестве ВЕРХНЕЙ для предыдущего по счёту rank
    MPI_Request recv_up_from_down; //отвечает за приём ВЕРХНЕЙ части, пришедшей из НИЖНЕЙ последующего по счёту rank
    MPI_Request send_up_to_down; //отвечает за доставку исходной ВЕРХНЕЙ части в качестве НИЖНЕЙ для предыдущего по счёту rank
    MPI_Request recv_down_from_up; //отвечает за приём НИЖНЕЙ части, пришедшей из ВЕРХНЕЙ последующего по счёту rank
    if (memory_init(argv, &N_i, &N_j, &N_k, &e, &a, &I_0, &J_0, &K_0, &D_i, &D_j, &D_k, &h_i, &h_j, &h_k, &h_i_2, &h_j_2, &h_k_2, &coefficient, &F, &up, &down, &swap1, &swap2, &complete, & complete_all, &count, size, &block) == 0)
    {
        init_F(F, block, rank, size, N_i, N_j, N_k, h_i, h_j, h_k, I_0, J_0, K_0);
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        while (complete_all == 0)
        {
            //Обмен границами
            if (rank != 0)
            {
                MPI_Isend(&(F[swap1][0]), N_j * N_k, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &send_down_to_up); //отправляем низ предыдущему блоку (для вверха)
                MPI_Irecv(down, N_j * N_k, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &recv_down_from_up); //примнимаем снизу вверх предыдущего блока
            }
            if (rank != size - 1)
            {
                MPI_Isend(&(F[swap1][(block - 1) * N_j * N_k]), N_j * N_k, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &send_up_to_down); //отправляем вверх следующему блоку (для низа)
                MPI_Irecv(up, N_j * N_k, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &recv_up_from_down); //принимаем вверх снизу следующего блока
            }
            //Считаем внутри
            iteration_in(F, block, rank, swap1, swap2, &complete, e, N_i, N_j, N_k, coefficient, a, h_i, h_j, h_k, h_i_2, h_j_2, h_k_2, I_0, J_0, K_0);
            //Ожидаем, пока они не перекинуться данными
            if (rank != 0)
            {
                MPI_Wait(&recv_down_from_up, MPI_STATUS_IGNORE);
                MPI_Wait(&send_down_to_up, MPI_STATUS_IGNORE);
            }
            if (rank != size - 1)
            {
                MPI_Wait(&recv_up_from_down, MPI_STATUS_IGNORE);
                MPI_Wait(&send_up_to_down, MPI_STATUS_IGNORE);
            }
            //Считаем края
            iteration_edge(F, up, down, block, rank, size, swap1, swap2, &complete, e, N_i, N_j, N_k, coefficient, a, h_i, h_j, h_k, h_i_2, h_j_2, h_k_2, I_0, J_0, K_0);
            //Собираем в кучу результаты
            MPI_Allreduce(&complete, &complete_all, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
            count++;
            swapping(&swap1, &swap2);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        double accuracy_ = accuracy(F, swap1, block, rank, a, N_i, N_j, N_k, h_i, h_j, h_k, I_0, J_0, K_0);
        double accuracy_max;
        double time = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        double time_min;
        MPI_Reduce(&time, &time_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&accuracy_, &accuracy_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0)
        {
            printf("count = %d\n", count);
            printf("Accuracy = %e\n", accuracy_max);
            printf("Time: %lf \n", time);
        }

        //Завершаемся
        free(F[0]);
        free(F[1]);
        free(F);
        free(up);
        free(down);
    }
    MPI_Finalize();
    return 0;
}