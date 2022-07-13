#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>
#include <pthread.h>

#pragma warning(disable : 4996)

//#define threads 6
int r = 1;
int threads = 1;
pthread_t* thread;
int to_a = 1;

double** F;
double** ro;
int Nx = 2000;
int Ny = 2000;
int Nt = 1000;
double Xa = 0.0;
double Xb = 4.0;
double Ya = 0.0;
double Yb = 4.0;
double X_ba;
double Y_ba;
double h_x;
double h_y;
double X_s1;
double Y_s1;
double X_s2;
double Y_s2;
double h_x2;
double h_y2;
double _h_x2_h_y2;
double X_h_x2_h_y2;
double Y_h_x2_h_y2;
double koef_1_5;
double koef_1_4;

__m256d v__X_h_x2_h_y2;
__m256d v__Y_h_x2_h_y2;
__m256d v__koef_1_5;
__m256d v__koef_1_4;

int* green;
int* red;
int* blue;

//int count_cells[threads] = {0};
//int c1[threads] = {0};
//int c2[threads] = {0};
//int c3[threads] = {0};

pthread_barrier_t barrier;

double min(double a, double b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

void print_massive(double* M, int Nx, int Ny)
{
    for (int i = 0; i <= Ny - 1; i++) // < ?
    {
        for (int j = 0; j <= Nx - 1; j++) // < ?
        {
            printf("%f\t", M[i * Nx + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void init_ro()
{
    double X_j;
    double Y_i;
    double R;
    double X_js1;
    double X_js2;
    double Y_is1;
    double Y_is2;

    for (int i = 0; i <= Ny - 1; i++) // < ?
    {
        for (int j = 0; j <= Nx - 1; j++) // < ?
        {
            X_j = Xa + j * h_x;
            Y_i = Ya + i * h_y;
            R = 0.1 * min(X_ba, Y_ba); //константа
            X_js1 = X_j - X_s1;
            Y_is1 = Y_i - Y_s1;
            if ((X_js1 * X_js1 + Y_is1 * Y_is1) < R * R)
            {
                ro[0][i * Nx + j] = 1.0;
            }
            else 
            {
                X_js2 = X_j - X_s2;
                Y_is2 = Y_i - Y_s2;
                if ((X_js2 * X_js2 + Y_is2 * Y_is2) < R * R)
                {
                    ro[0][i * Nx + j] = -1.0;
                }
                else 
                {
                    ro[0][i * Nx + j] = 0.0;
                }
            }
        }
    }
    for (int i = 1; i <= Ny - 2; i++) // < ?
    {
        for (int j = 1; j <= Nx - 2; j++) // < ?
        {
            ro[1][i * Nx + j] = 2 * ro[0][i * Nx + j] + 0.25 * (ro[0][i * Nx + j + 1] + ro[0][i * Nx + j - 1] + ro[0][(i + 1) * Nx + j] + ro[0][(i - 1) * Nx + j]);
        }
    }
}

void mem_v_step(double* F_new, double* F_old, double* ro, int i, int j, int thread_id)
{
    //count_cells[thread_id]++;
    __m256d frist1;
    __m256d second1;
    __m256d frist2;
    __m256d second2;
    __m256d frist3;
    __m256d second3;
    __m256d thrid3;
    __m256d fouth3;
    __m256d frist5;
    __m256d second5;
    __m256d result1_1;
    __m256d result1_2;
    __m256d result2_1;
    __m256d result2_2;
    __m256d result3_1;
    __m256d result3_2;
    __m256d result3_3;
    __m256d result3_4;
    __m256d result4_1;
    __m256d result5_1;
    __m256d total;
    
    frist1 = _mm256_load_pd(F_old + i * Nx + j - 1);
    second1 = _mm256_load_pd(F_old + i * Nx + j + 1);
    result1_1 = _mm256_add_pd(frist1, second1);
    result1_2 = _mm256_mul_pd(v__X_h_x2_h_y2, result1_1);

    //Y_h_x2_h_y2 * (F_old[i * Nx + j + Nx] + F_old[i * Nx + j - Nx])
    frist2 = _mm256_load_pd(F_old + i * Nx + j + Nx);
    second2 = _mm256_loadu_pd(F_old + i * Nx + j - Nx);
    result2_1 = _mm256_add_pd(frist2, second2);
    result2_2 = _mm256_mul_pd(v__Y_h_x2_h_y2, result2_1);
   
    //koef_1_4 * (F_old[i * Nx + j - Nx - 1] + F_old[i * Nx + j - Nx + 1] + F_old[i * Nx + j + Nx - 1] + F_old[i * Nx + j + Nx + 1])
    frist3 = _mm256_load_pd(F_old + i * Nx + j - Nx - 1);
    second3 = _mm256_loadu_pd(F_old + i * Nx + j - Nx + 1);
    thrid3 = _mm256_load_pd(F_old + i * Nx + j + Nx - 1);
    fouth3 = _mm256_load_pd(F_old + i * Nx + j + Nx + 1);
    result3_1 = _mm256_add_pd(frist3, second3);
    result3_2 = _mm256_add_pd(thrid3, fouth3);
    result3_3 = _mm256_add_pd(result3_1, result3_2);
    result3_4 = _mm256_mul_pd(v__koef_1_4, result3_3);

    //ro[i * Nx + j]
    result4_1 = _mm256_loadu_pd(ro + i * Nx + j);

    //Sum results
    frist5 = _mm256_add_pd(result1_2, result2_2);
    second5 = _mm256_add_pd(result3_4, result4_1);
    result5_1 = _mm256_add_pd(frist5, second5);
    total = _mm256_mul_pd(v__koef_1_5, result5_1);
    _mm256_storeu_pd(F_new + i * Nx + j, total);
}

//#define pthread_barrier_wait(x)

void one_thread(int thread_id)
{
    for (int it = 0; it < Nt; it += r)
    {
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1), l = 1; l <= k; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c1[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                }
            }
        }
        for (int p = red[thread_id * 16]; p <= blue[thread_id * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c2[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p - (l - 1), j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16], l = k; l <= r; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j += 4)
                {
                    //c3[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void first(int thread_id)
{
    for (int it = 0; it < Nt; it += r)
    {
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1), l = 1; l <= k; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c1[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                }
            }
        }
        for (int p = red[thread_id * 16]; p <= blue[thread_id * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c2[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p - (l - 1), j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16] + (k - 2), l = k; l <= r; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j += 4)
                {
                    //c3[thread_id] += 2;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p + 1, j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void middle(int thread_id)
{
    for (int it = 0; it < Nt; it += r)
    {
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1) * 2, l = 1; l <= k; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c1[thread_id] += 2;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p + 1, j, thread_id);
                }
            }
        }
        for (int p = red[thread_id * 16]; p <= blue[thread_id * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c2[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p - (l - 1), j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16] + (k - 2), l = k; l <= r; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j += 4)
                {
                    //c3[thread_id] += 2;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p + 1, j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void last(int thread_id)
{
    for (int it = 0; it < Nt; it += r)
    {
        for (int k = 1; k < r; k++)
        {
            for (int p = green[thread_id * 16] + (k - 1) * 2, l = 1; l <= k; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c1[thread_id] += 2;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p + 1, j, thread_id);
                }
            }
        }
        for (int p = red[thread_id * 16]; p <= blue[thread_id * 16]; p++)
        {
            for (int l = 1; l <= r; l++)
            {
                for (int j = 1; j <= Nx - 2; j+=4)
                {
                    //c2[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p - (l - 1), j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
        for (int k = 2; k <= r; k++)
        {
            for (int p = blue[thread_id * 16], l = k; l <= r; p--, l++)
            {
                for (int j = 1; j <= Nx - 2; j += 4)
                {
                    //c3[thread_id]++;
                    mem_v_step(F[(it + l) % 2], F[(it + l - 1) % 2], ro[1], p, j, thread_id);
                }
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void* new_thread(void* param)
{
    int thread_id = *((int*)param);
    free((int*)param);
    //printf("[ %d | %d | new_thread()] Thread started!\n", thread_id, (int)pthread_self());

    if (threads == 1)
    {
        one_thread(thread_id);
    }
    else if (thread_id == 0)
    {
        first(thread_id);
    } 
    else if (thread_id == threads - 1)
    {
        last(thread_id);
    }
    else 
    {
        middle(thread_id);
    }

    //printf("[ %d ] finish\n", thread_id);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    if (argv[1] != NULL) 
    {
        threads = atoi(argv[1]);
    }
    if ((argv[1] != NULL) && (argv[2] != NULL))
    {
        r = atoi(argv[2]);
    }
    if ((argv[1] != NULL) && (argv[2] != NULL) && (argv[3] != NULL))
    {
        to_a = atoi(argv[3]);
    } 

    X_ba = Xb - Xa;
    Y_ba = Yb - Ya;
    h_x = X_ba / (Nx - 1);
    h_y = Y_ba / (Ny - 1);
    X_s1 = Xa + X_ba / 3.0;
    Y_s1 = Ya + Y_ba * (2.0 / 3.0);
    X_s2 = Xa + X_ba * (2.0 / 3.0);
    Y_s2 = Ya + Y_ba / 3.0;
    h_x2 = h_x * h_x;
    h_y2 = h_y * h_y;
    _h_x2_h_y2 = 1.0 / h_x2 + 1.0 / h_y2;
    X_h_x2_h_y2 = 0.5 * (5.0 / h_x2 - 1.0 / h_y2);
    Y_h_x2_h_y2 = 0.5 * (5.0 / h_y2 - 1.0 / h_x2);
    koef_1_5 = 0.2 * (1.0 / _h_x2_h_y2);
    koef_1_4 = 0.25 * _h_x2_h_y2;

    v__X_h_x2_h_y2 = _mm256_set1_pd(X_h_x2_h_y2);
    v__Y_h_x2_h_y2 = _mm256_set1_pd(Y_h_x2_h_y2);
    v__koef_1_5 = _mm256_set1_pd(koef_1_5);
    v__koef_1_4 = _mm256_set1_pd(koef_1_4);

    double delta = 999999999.0;
    double delta_new;
    
    double best_time = 9999.9;
    for (int a = 0; a < to_a; a++)
    {
        #ifdef pthread_barrier_wait
            printf("no barriers; ");
        #else
            printf("with barriers; ");
        #endif

        F = (double**)calloc(2, sizeof(double*));
        F[0] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
        F[1] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
        memset(F[0], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
        memset(F[1], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам

        ro = (double**)calloc(2, sizeof(double*));
        ro[0] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
        ro[1] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
        memset(ro[0], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
        memset(ro[1], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
        
        init_ro();

        green = (int*)calloc(threads * 16, sizeof(int));
        red = (int*)calloc(threads * 16, sizeof(int));
        blue = (int*)calloc(threads * 16, sizeof(int));
        memset(green, 0, threads * 16*  sizeof(int));
        memset(red, 0, threads * 16 * sizeof(int));
        memset(blue, 0, threads * 16 * sizeof(int));
        for (int i = 0; i < threads; i++)
        {
            green[i * 16] = i * (Ny / threads) - (r - 1);
            red[i * 16] = i * (Ny / threads) + (r - 1);
            blue[i * 16] = (i + 1) * (Ny / threads) - r;
        }
        green[0] = 1;
        red[0]++;
        blue[(threads - 1) * 16] = Ny - 2;

        /*
        printf("green: ");
        for (int i = 0; i < threads; i++)
        {
            printf("%d; ", green[i]);
        }
        printf("\n"); 
        printf("red: ");
        for (int i = 0; i < threads; i++)
        {
            printf("%d; ", red[i]);
        }
        printf("\n"); 
        printf("blue: ");
        for (int i = 0; i < threads; i++)
        {
            printf("%d; ", blue[i]);
        }
        printf("\n"); 
        */

        pthread_barrier_init(&barrier, NULL, threads);

        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        //pthread_t thread[threads]; //= (pthread_t*)calloc(thread_pool, sizeof(pthread_t));
        thread = (pthread_t*)calloc(threads, sizeof(pthread_t));
        for (int i = 0; i < threads; i++)
        {
            int* thread_ID = (int*)malloc(sizeof(int));
            (*thread_ID) = i;
            int error_code = pthread_create(&thread[i], NULL, new_thread, thread_ID);
            if (error_code != 0)
            {
                printf("[MAIN | Waring] Error in create thread.");
            }
        }

        for (int i = 0; i < threads; i++)
        {
            pthread_join(thread[i], NULL);
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &finish);

        int it = Nt;
        if (Nt % r != 0)
        {
            int a = Nt / r;
            it = a * r + r;
        }

        /*
        int t_count_cells = 0, t_c1 = 0, t_c2 = 0, t_c3 = 0;
        for (int i = 0; i < threads; i++)
        {
            t_count_cells += count_cells[i];
            t_c1 += c1[i];
            t_c2 += c2[i];
            t_c3 += c3[i];
        }
        */

        //FILE* file = fopen("file", "wb");
        //fwrite(F[it % 2], sizeof(double), Nx * Ny, file);

        double time = finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec);
        double true_time = time / (double)it * (double)Nt;
        //int true_count = t_count_cells / it * Nt;
        //printf("threads = %d; Nx = %d, Ny = %d, Nt = %d, r = %d; it = %d; time = %lf; true time = %lf; total count = %d; true count = %d; count 1 = %d, count 2 = %d, count 3 = %d\n", threads, Nx, Ny, Nt, r, it, time, true_time, t_count_cells, true_count, t_c1, t_c2, t_c3);
        printf("threads = %d; r = %d; Nx = %d; Ny = %d; Nt = %d; it = %d; time = %lf; true time = %lf\n", threads, r, Nx, Ny, Nt, it, time, true_time);
        
        pthread_barrier_destroy(&barrier);
        free(F[0]);
        free(F[1]);
        free(ro[0]);
        free(ro[1]);
        free(green);
        free(red);
        free(blue);
        free(thread);

        if (true_time < best_time)
        {
            best_time = true_time;
        }
    }

    FILE* stat = fopen("statistic.txt", "a");
    fprintf(stat, "%lf; ", best_time);
    fclose(stat);
    printf("threads = %d; r = %d; Nx = %d; Ny = %d; Nt = %d; a = %d; best time = %lf\n", threads, r, Nx, Ny, Nt, to_a, best_time);

    return 0;
}
