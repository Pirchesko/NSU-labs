#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <mpe.h>

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

int main(int argc, char* argv[])
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
    for (int i = 0; i < N; i++)
    {
        b[i] = 0;
        for (int j = 0; j < N; j++)
        {
            b[i] += A[i * N + j] * x[j];
        }
    }
    random_vector(x, N);
    //Инициализируем профилировщик
    MPI_Init(&argc, &argv);
    MPE_Init_log();
    int id_begin_phase_1 = MPE_Log_get_event_number();
    int id_end_phase_1 = MPE_Log_get_event_number();
    int id_begin_phase_2 = MPE_Log_get_event_number();
    int id_end_phase_2 = MPE_Log_get_event_number();
    //int id_begin_phase_3 = MPE_Log_get_event_number();
    //int id_end_phase_3 = MPE_Log_get_event_number();
    //int id_begin_phase_4 = MPE_Log_get_event_number();
    //int id_end_phase_4 = MPE_Log_get_event_number();
    //int id_begin_phase_5 = MPE_Log_get_event_number();
    //int id_end_phase_5 = MPE_Log_get_event_number();
    //int id_begin_phase_6 = MPE_Log_get_event_number();
    //int id_end_phase_6 = MPE_Log_get_event_number();
    //int id_begin_phase_7 = MPE_Log_get_event_number();
    //int id_end_phase_7 = MPE_Log_get_event_number();
    //int id_begin_phase_8 = MPE_Log_get_event_number();
    //int id_end_phase_8 = MPE_Log_get_event_number();
    //int id_begin_phase_9 = MPE_Log_get_event_number();
    //int id_end_phase_9 = MPE_Log_get_event_number();
    MPE_Describe_state(id_begin_phase_1, id_end_phase_1, "Phase_1", "red");
    MPE_Describe_state(id_begin_phase_2, id_end_phase_2, "Phase_2", "green");
    //MPE_Describe_state(id_begin_phase_3, id_end_phase_3, "Phase_3", "blue");
    //MPE_Describe_state(id_begin_phase_4, id_end_phase_4, "Phase_4", "purple");
    //MPE_Describe_state(id_begin_phase_5, id_end_phase_5, "Phase_5", "yellow");
    //MPE_Describe_state(id_begin_phase_6, id_end_phase_6, "Phase_6", "orange");
    //MPE_Describe_state(id_begin_phase_7, id_end_phase_7, "Phase_7", "black");
    //MPE_Describe_state(id_begin_phase_8, id_end_phase_8, "Phase_8", "white");
    //MPE_Describe_state(id_begin_phase_9, id_end_phase_9, "Phase_9", "grey");
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
                MPE_Log_event(id_begin_phase_1, omp_get_thread_num(), (char*)0);
                sum1 = 0.0;
                sum2 = 0.0;
                sum3 = 0.0;
                sum4 = 0.0;
                MPE_Log_event(id_end_phase_1, omp_get_thread_num(), (char*)0);
            }
            #pragma omp for reduction(+: sum1, sum2) 
            for (int i = 0; i < N; i++)
            {
                MPE_Log_event(id_begin_phase_2, omp_get_thread_num(), (char*)0);
                Ax[i] = 0;
                for (int j = 0; j < N; j++)
                {
                    Ax[i] += A[i * N + j] * x[j];
                }
                y[i] = Ax[i] - b[i];
                sum1 += y[i] * y[i];
                sum2 += b[i] * b[i];
                MPE_Log_event(id_end_phase_2, omp_get_thread_num(), (char*)0);
            }
            #pragma omp single
            {
                //MPE_Log_event(id_begin_phase_3, omp_get_thread_num(), (char*)0);
                if ((sqrt(sum1) / sqrt(sum2)) < epsilon)
                {
                    check = 0;
                }
                //MPE_Log_event(id_end_phase_3, omp_get_thread_num(), (char*)0);
            }
            if (check == 1)
            {
                #pragma omp for reduction(+: sum3, sum4)
                for (int i = 0; i < N; i++)
                {
                    //MPE_Log_event(id_begin_phase_4, omp_get_thread_num(), (char*)0);
                    Ay[i] = 0;
                    for (int j = 0; j < N; j++)
                    {
                        Ay[i] += A[i * N + j] * y[j];
                    }
                    sum3 += y[i] * Ay[i];
                    sum4 += Ay[i] * Ay[i];
                    //MPE_Log_event(id_end_phase_4, omp_get_thread_num(), (char*)0);
                }
                #pragma omp single
                {
                    //MPE_Log_event(id_begin_phase_5, omp_get_thread_num(), (char*)0);
                    t = sum3 / sum4;
                    //MPE_Log_event(id_end_phase_5, omp_get_thread_num(), (char*)0);
                }
                #pragma omp for 
                for (int i = 0; i < N; i++)
                {
                    //MPE_Log_event(id_begin_phase_6, omp_get_thread_num(), (char*)0);
                    ty[i] = t * y[i];
                    x[i] = x[i] - ty[i];
                    //MPE_Log_event(id_end_phase_6, omp_get_thread_num(), (char*)0);
                }
                #pragma omp single
                {
                    //MPE_Log_event(id_begin_phase_7, omp_get_thread_num(), (char*)0);
                    k++;
                    //MPE_Log_event(id_end_phase_7, omp_get_thread_num(), (char*)0);
                }
            }
        }
    }
    //Фиксируем время конца
    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    printf("%d; ", k);
    printf("%lf; \n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
    MPE_Finish_log("lab2_2_mpe.clog2");
    MPI_Finalize();
    free(A);
    free(x);
    free(b);
    free(Ax);
    free(Ay);
    free(ty);
    free(y);
    return 0;
}