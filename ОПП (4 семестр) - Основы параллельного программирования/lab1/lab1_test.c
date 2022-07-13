#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//----------Operations----------

// |x|=sqrt(sum(x_i^2))
double modul(double* x, int N)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        sum += x[i] * x[i];
    }
    return sqrt(sum);
}

// (x,y)=sum(x_i*y_i)
double skobki(double* x, double* y, int N)
{
    double res = 0.0;
    for (int i = 0; i < N; i++)
    {
        res += x[i] * y[i];
    }
    return res;
}

//multiplicate matrix to vector: result = x * y (N - size; x - matrix; result, y - vector)
void mul_M_v(double* result, double* x, double* y, int N)
{
    double* res = (double*)malloc(N * sizeof(double));
    for (int i = 0; i < N; i++)
    {
        res[i] = 0.0;
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            res[i] += x[i * N + j] * y[j];
        }
    }
    for (int i = 0; i < N; i++)
    {
        result[i] = res[i];
    }
    free(res);
}

//multiplicate constant to vector: result = c * y (N - size; c - constant; result, y - vector)
void mul_c_v(double* result, double c, double* y, int N)
{
    for (int i = 0; i < N; i++)
    {
        result[i] = c * y[i];
    }
}

//substraction vector to vector: result = x - y (N - size; result, x, y - vector)
void sub(double* result, double* x, double* y, int N)//elited!!!
{
    //double* res = (double*)malloc(N * sizeof(double));
    //for (int i = 0; i < N; i++)
    //{
    //    res[i] = 0.0;
    //}
    for (int i = 0; i < N; i++)
    {
        result[i] = x[i] - y[i];
    }
    //for (int i = 0; i < N; i++)
    //{
    //    result[i] = res[i];
    //}
    //free(res);
}


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


//----------Algorithm----------
void y_pow(double* y, double* A, double* x, double* b, int N)
{
    double* Ax = (double*)malloc(N * sizeof(double));
    mul_M_v(Ax, A, x, N);
    sub(y, Ax, b, N);
    free(Ax);
}

double t_pow(double* y, double* A, int N)
{
    double* Ay = (double*)malloc(N * sizeof(double));
    mul_M_v(Ay, A, y, N);
    double t = skobki(y, Ay, N) / skobki(Ay, Ay, N);
    free(Ay);
    return t;
}

void x_pow(double* y, double* A, double* x, double* b, int N)
{
    y_pow(y, A, x, b, N);
    double t = t_pow(y, A, N);
    double* ty = (double*)malloc(N * sizeof(double));
    mul_c_v(ty, t, y, N);
    sub(x, x, ty, N);
    free(ty);
}

int test(double* y, double* b, double epsilon, int N)
{
    double t = modul(y, N) / modul(b, N);
    if (t < epsilon)
    {
        return 1; //остановка
    }
    else
    {
        return 0; //продолжай, пока не 1
    }
}

int main()
{
    //Входные данные
     //чтобы входные данные при каждом запуске были разные
    for (int j = 3364; j <= 6000; j++)
    {
        srand(j);
        int N = 1024;
        double epsilon = 0.1;
        double* A = (double*)malloc(N * N * sizeof(double));
        double* x = (double*)malloc(N * sizeof(double));
        double* b = (double*)malloc(N * sizeof(double));
        double* y = (double*)malloc(N * sizeof(double)); // y=Ax-b
        //Создаём матрицу A и x, считаем b, а затем переписываем x (как раз далее будем этот вектор находить)
        symmetrical_matrix(A, N);
        //print_matrix(A, N);
        random_vector(x, N);
        //printf("Answer x = ");
        //print_vector(x, N);
        mul_M_v(b, A, x, N);
        random_vector(x, N);
        y_pow(y, A, x, b, N);
        //Начало алгоритма
        int i = 0; //счётчик итераций
        while (test(y, b, epsilon, N) == 0)
        {
            x_pow(y, A, x, b, N);
            i++;
            if (i > 350)
            {
                printf("%5d: Steps over than 350!\n", j);
                break;
            }
        }
        //print_vector(x, N);
        if (i < 350)
        {
            printf("%5d: Steps: %d\n", j, i);
        }
        free(A);
        free(x);
        free(b);
        free(y);
    }
    
    return 0;
}

//http://rsusu1.rnd.runnet.ru/tutor/method/m2/page09.html
//https://www.opennet.ru/docs/RUS/MPI_intro/#Barrier