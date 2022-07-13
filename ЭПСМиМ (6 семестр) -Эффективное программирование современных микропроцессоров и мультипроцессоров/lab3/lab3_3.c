#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>

#pragma warning(disable : 4996)

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

void init_ro(double** ro, int Nx, int Ny, double X_s1, double X_s2, double Y_s1, double Y_s2, double h_x, double h_y, double Xa, double Ya, double X_ba, double Y_ba)
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

double steps(double* F_new, double* F_old, double* ro, double X_h_x2_h_y2, double Y_h_x2_h_y2, double koef_1_5, double koef_1_4, int Nx, int Ny)
{
    double max = 0.0;
    double delta_ij;
    for (int i = 1; i <= Ny - 2; i++) // < ?
    {
        for (int j = 1; j <= Nx - 2; j++) // < ?
        {
            F_new[i * Nx + j] = koef_1_5 * (
                X_h_x2_h_y2 * (F_old[i * Nx + j - 1] + F_old[i * Nx + j + 1])
                + Y_h_x2_h_y2 * (F_old[i * Nx + j + Nx] + F_old[i * Nx + j - Nx])
                + koef_1_4 * (F_old[i * Nx + j - Nx - 1] + F_old[i * Nx + j - Nx + 1] + F_old[i * Nx + j + Nx - 1] + F_old[i * Nx + j + Nx + 1])
                + ro[i * Nx + j] //old sdvig
            );
            delta_ij = fabs(F_new[i * Nx + j] - F_old[i * Nx + j]);
            if (max < delta_ij)
            {
                max = delta_ij;
            }
        }
    }
    //print_massive(F_new, Nx, Ny);
    return max;
}

double v_steps(double* F_new, double* F_old, double* ro, __m256d X_h_x2_h_y2, __m256d Y_h_x2_h_y2, __m256d koef_1_5, __m256d koef_1_4, int Nx, int Ny)
{
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

    for (int i = 1; i <= Ny - 2; i++) // < ?
    {
        for (int j = 1; j <= Nx - 2; j += 4) // < ?
        {
            //X_h_x2_h_y2 * (F_old[i * Nx + j - 1] + F_old[i * Nx + j + 1])
            frist1 = _mm256_load_pd(F_old + i * Nx + j - 1);
            second1 = _mm256_load_pd(F_old + i * Nx + j + 1);
            result1_1 = _mm256_add_pd(frist1, second1);
            result1_2 = _mm256_mul_pd(X_h_x2_h_y2, result1_1);

            //Y_h_x2_h_y2 * (F_old[i * Nx + j + Nx] + F_old[i * Nx + j - Nx])
            frist2 = _mm256_load_pd(F_old + i * Nx + j + Nx);
            second2 = _mm256_loadu_pd(F_old + i * Nx + j - Nx);
            result2_1 = _mm256_add_pd(frist2, second2);
            result2_2 = _mm256_mul_pd(Y_h_x2_h_y2, result2_1);
            
            //koef_1_4 * (F_old[i * Nx + j - Nx - 1] + F_old[i * Nx + j - Nx + 1] + F_old[i * Nx + j + Nx - 1] + F_old[i * Nx + j + Nx + 1])
            frist3 = _mm256_load_pd(F_old + i * Nx + j - Nx - 1);
            second3 = _mm256_load_pd(F_old + i * Nx + j - Nx + 1);
            thrid3 = _mm256_load_pd(F_old + i * Nx + j + Nx - 1);
            fouth3 = _mm256_load_pd(F_old + i * Nx + j + Nx + 1);
            result3_1 = _mm256_add_pd(frist3, second3);
            result3_2 = _mm256_add_pd(thrid3, fouth3);
            result3_3 = _mm256_add_pd(result3_1, result3_2);
            result3_4 = _mm256_mul_pd(koef_1_4, result3_3);

            //ro[i * Nx + j]
            result4_1 = _mm256_load_pd(ro + i * Nx + j);

            //Sum results
            frist5 = _mm256_add_pd(result1_2, result2_2);
            second5 = _mm256_add_pd(result3_4, result4_1);
            result5_1 = _mm256_add_pd(frist5, second5);
            total = _mm256_mul_pd(koef_1_5, result5_1);
            _mm256_storeu_pd(F_new + i * Nx + j, total);


            //delta_ij = fabs(F_new[i * Nx + j] - F_old[i * Nx + j]);
            //if (max < delta_ij)
            //{
            //   max = delta_ij;
            //}
        }
    }
    //print_massive(F_new, Nx, Ny);
    double max1 = 0.0;
    return max1;
}

void mem_v_step(double* F_new, double* F_old, double* ro, __m256d X_h_x2_h_y2, __m256d Y_h_x2_h_y2, __m256d koef_1_5, __m256d koef_1_4, int Nx, int Ny, int i, int j/*, __m256d frist1,__m256d second1, __m256d frist2, __m256d second2, __m256d frist3, __m256d second3, __m256d thrid3, __m256d fouth3, __m256d frist5, __m256d second5, __m256d result1_1, __m256d result1_2, __m256d result2_1, __m256d result2_2, __m256d result3_1, __m256d result3_2, __m256d result3_3, __m256d result3_4, __m256d result4_1, __m256d result5_1, __m256d total*/)
{
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
    result1_2 = _mm256_mul_pd(X_h_x2_h_y2, result1_1);

    //Y_h_x2_h_y2 * (F_old[i * Nx + j + Nx] + F_old[i * Nx + j - Nx])
    frist2 = _mm256_load_pd(F_old + i * Nx + j + Nx);
    second2 = _mm256_loadu_pd(F_old + i * Nx + j - Nx);
    result2_1 = _mm256_add_pd(frist2, second2);
    result2_2 = _mm256_mul_pd(Y_h_x2_h_y2, result2_1);
   
    //koef_1_4 * (F_old[i * Nx + j - Nx - 1] + F_old[i * Nx + j - Nx + 1] + F_old[i * Nx + j + Nx - 1] + F_old[i * Nx + j + Nx + 1])
    frist3 = _mm256_load_pd(F_old + i * Nx + j - Nx - 1);
    second3 = _mm256_loadu_pd(F_old + i * Nx + j - Nx + 1);
    thrid3 = _mm256_load_pd(F_old + i * Nx + j + Nx - 1);
    fouth3 = _mm256_load_pd(F_old + i * Nx + j + Nx + 1);
    result3_1 = _mm256_add_pd(frist3, second3);
    result3_2 = _mm256_add_pd(thrid3, fouth3);
    result3_3 = _mm256_add_pd(result3_1, result3_2);
    result3_4 = _mm256_mul_pd(koef_1_4, result3_3);

    //ro[i * Nx + j]
    result4_1 = _mm256_loadu_pd(ro + i * Nx + j);

    //Sum results
    frist5 = _mm256_add_pd(result1_2, result2_2);
    second5 = _mm256_add_pd(result3_4, result4_1);
    result5_1 = _mm256_add_pd(frist5, second5);
    total = _mm256_mul_pd(koef_1_5, result5_1);
    _mm256_storeu_pd(F_new + i * Nx + j, total);
}

int main()
{
    int id = 0;
    int Nx = 8000;
    int Ny = 8000;
    int Nt = 100;
    double Xa = 0.0;
    double Xb = 4.0;
    double Ya = 0.0;
    double Yb = 4.0;
    double X_ba = Xb - Xa;
    double Y_ba = Yb - Ya;
    double h_x = X_ba / (Nx - 1);
    double h_y = Y_ba / (Ny - 1);
    double X_s1 = Xa + X_ba / 3.0;
    double Y_s1 = Ya + Y_ba * (2.0 / 3.0);
    double X_s2 = Xa + X_ba * (2.0 / 3.0);
    double Y_s2 = Ya + Y_ba / 3.0;
    double h_x2 = h_x * h_x;
    double h_y2 = h_y * h_y;
    double _h_x2_h_y2 = 1.0 / h_x2 + 1.0 / h_y2;
    double X_h_x2_h_y2 = 0.5 * (5.0 / h_x2 - 1.0 / h_y2);
    double Y_h_x2_h_y2 = 0.5 * (5.0 / h_y2 - 1.0 / h_x2);
    double koef_1_5 = 0.2 * (1.0 / _h_x2_h_y2);
    double koef_1_4 = 0.25 * _h_x2_h_y2;

    __m256d v__X_h_x2_h_y2 = _mm256_set1_pd(X_h_x2_h_y2);
    __m256d v__Y_h_x2_h_y2 = _mm256_set1_pd(Y_h_x2_h_y2);
    __m256d v__koef_1_5 = _mm256_set1_pd(koef_1_5);
    __m256d v__koef_1_4 = _mm256_set1_pd(koef_1_4);

    double delta = 999999999.0;
    double delta_new;

    double** F = (double**)calloc(2, sizeof(double*));
    F[0] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
    F[1] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
    memset(F[0], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
    memset(F[1], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам

    double** ro = (double**)calloc(2, sizeof(double*));
    ro[0] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
    ro[1] = (double*)_mm_malloc(Nx*Ny*sizeof(double), 32);
    memset(ro[0], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
    memset(ro[1], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам

    init_ro(ro, Nx, Ny, X_s1, X_s2, Y_s1, Y_s2, h_x, h_y, Xa, Ya, X_ba, Y_ba);
    
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    int it, i, j;
    for (it = 0; it < Nt; it += 3)
    {
        i = 1;
        for (j = 1; j <= Nx - 2; j+=4)
        {
            mem_v_step(F[(id + 1) % 2], F[(id + 0) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i, j);
        }
        i = 2;
        for (j = 1; j <= Nx - 2; j += 4) // < ?
        {
            mem_v_step(F[(id + 1) % 2], F[(id + 0) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i, j);
        }
        for (j = 1; j <= Nx - 2; j += 4) // < ?
        {
            mem_v_step(F[(id + 2) % 2], F[(id + 1) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 1, j);
        }

        for (i = 3; i <= Ny - 2; i++) // < ?
        {
            for (j = 1; j <= Nx - 2; j += 4) // < ?
            {
                mem_v_step(F[(id + 1) % 2], F[(id + 0) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i, j);
            }
            for (j = 1; j <= Nx - 2; j += 4) // < ?
            {
                mem_v_step(F[(id + 2) % 2], F[(id + 1) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 1, j);
            }
            for (j = 1; j <= Nx - 2; j += 4) // < ?
            {
                mem_v_step(F[(id + 3) % 2], F[(id + 2) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 2, j);
            }
        }

        i = Ny - 1;
        for (j = 1; j <= Nx - 2; j += 4) // < ?
        {
            mem_v_step(F[(id + 2) % 2], F[(id + 1) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 1, j);
        }
        for (j = 1; j <= Nx - 2; j += 4) // < ?
        {
            mem_v_step(F[(id + 3) % 2], F[(id + 2) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 2, j);
        }

        i = Ny - 1;
        for (j = 1; j <= Nx - 2; j+=4)
        {
            mem_v_step(F[(id + 3) % 2], F[(id + 2) % 2], ro[1], v__X_h_x2_h_y2, v__Y_h_x2_h_y2, v__koef_1_5, v__koef_1_4, Nx, Ny, i - 1, j);
        }

        if (delta < delta_new)
        {
            printf("Waring... [%d] delta_now = %e, delta_prev = %e\n", i, delta_new, delta);
        }
        delta = delta_new;
        id += 3;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);

    FILE* file = fopen("file", "wb");
    fwrite(F[id % 2], sizeof(double), Nx * Ny, file);

    printf("Time: %lf\n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
    printf("delta = %e; Nx = %d, Ny = %d, Nt = %d\n", delta, Nx, Ny, Nt);

    return 0;
}