#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>

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

void init_ro(double* ro, int Nx, int Ny, double X_s1, double X_s2, double Y_s1, double Y_s2, double h_x, double h_y, double Xa, double Ya, double X_ba, double Y_ba)
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
                ro[i * Nx + j] = 1.0;
            }
            else 
            {
                X_js2 = X_j - X_s2;
                Y_is2 = Y_i - Y_s2;
                if ((X_js2 * X_js2 + Y_is2 * Y_is2) < R * R)
                {
                    ro[i * Nx + j] = -1.0;
                }
            }
        }
    }
}

double steps(double* F_new, double* F_old, double* ro, double h_x, double h_y, int Nx, int Ny)
{
    double index_i;
    int index;
    int ip1_j;
    int im1_j;
    int i_jp1;
    int i_jm1;
    double max = 0.0;
    double delta_ij;
    for (int i = 1; i <= Ny - 2; i++) // < ?
    {
        index_i = i * Nx;
        for (int j = 1; j <= Nx - 2; j++) // < ?
        {
            index = index_i + j;
            ip1_j = index + Nx;
            im1_j = index - Nx;
            i_jp1 = index + 1;
            i_jm1 = index - 1;
            F_new[index] = 0.2 * (1.0 / (1.0 / (h_x * h_x) + 1.0 / (h_y * h_y))) * (
                (0.5 * (5.0 / (h_x * h_x) - 1.0 / (h_y * h_y))) * (F_old[i_jm1] + F_old[i_jp1])
                + (0.5 * (5.0 / (h_y * h_y) - 1.0 / (h_x * h_x))) * (F_old[ip1_j] + F_old[im1_j])
                + 0.25 * (1.0 / (h_x * h_x) + 1.0 / (h_y * h_y)) * (F_old[im1_j - 1] + F_old[im1_j + 1] + F_old[ip1_j - 1] + F_old[ip1_j + 1])
                + 2.0 * ro[index] + 0.25 * (ro[im1_j] + ro[ip1_j] + ro[i_jm1] + ro[i_jp1]) //old sdvig
            );
            delta_ij = fabs(F_new[index] - F_old[index]);
            if (max < delta_ij)
            {
                max = delta_ij;
            }
        }
    }
    //print_massive(F_new, Nx, Ny);
    return max;
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
    double Y_s1 = Ya + Y_ba * 2.0 / 3.0;
    double X_s2 = Xa + X_ba * 2.0 / 3.0;
    double Y_s2 = Ya + Y_ba / 3.0;

    double delta = 999999999.0;
    double delta_new;

    double** F = (double**)calloc(2, sizeof(double*));
    F[0] = (double*)calloc(Nx * Ny, sizeof(double));
    F[1] = (double*)calloc(Nx * Ny, sizeof(double));
    memset(F[0], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам
    memset(F[1], 0.0, Nx * Ny * sizeof(double)); //надо только по бокам

    double* ro = (double*)calloc(Nx * Ny, sizeof(double));
    memset(ro, 0.0, Nx * Ny * sizeof(double));

    init_ro(ro, Nx, Ny, X_s1, X_s2, Y_s1, Y_s2, h_x, h_y, Xa, Ya, X_ba, Y_ba);
    
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int i = 0; i < Nt; i++)
    {
        delta_new = steps(F[(id + 1) % 2], F[id % 2], ro, h_x, h_y, Nx, Ny);
        //print_massive(F[(id + 1) % 2], Nx, Ny);
        //printf("[%d] %lf\n", i, delta_new);
        if (delta < delta_new)
        {
            //printf("Waring... [%d] delta_now = %lf, delta_prev = %lf\n", i, delta_new, delta);
        }
        delta = delta_new;
        id++;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    printf("Time: %lf\n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
    printf("delta = %e; Nx = %d, Ny = %d, Nt = %d\n", delta, Nx, Ny, Nt);

    return 0;
}
