#include <iostream>
#include <cmath>
#include <memory>
#include <xmmintrin.h>
#include <immintrin.h>

const int ALIGN_INDENT = 64;
const int VECTOR_SIZE = 8;

//f0 = 1.0, t0 = 1.5, γ = 4.0
float f(float f0, float n, float tau, float t0, float fi) {
    return expf(-powf(2 * M_PI * f0 * (n * tau - t0), 2) / powf(fi, 2)) * sinf(2 * M_PI * f0 * (n * tau - t0));
}

void set(float *arr, int nx, int i, int j, float val) {
    *(arr + i * nx + j) = val;
}

float get(const float *u, int nx, int i, int j) {
    return *(u + i * nx + j);
}

float *getV(const float *u, int nx, int i, int j) {
    return (float *) (u + i * nx + j);
}

void insertValuesInP(float *p, int nx, int ny) {
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny / 2; j++) {
            set(p, nx, i, j, 0.01);
        }
    }

    for (int i = 0; i < nx; i++) {
        for (int j = ny / 2; j < ny; j++) {
            set(p, nx, i, j, 0.04);
        }
    }
}

void swapPtr(float **a, float **b) {
    float *tmp = *a;
    *a = *b;
    *b = tmp;
}

void printArray(float *arr, int nx, int ny, char *title) {
    std::cout << "------------" << title << "-----------------------\n";
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            std::cout << *(arr + i * nx + j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------------\n";
}

void printArrayInFile(float *arr, int nx, int ny) {
    FILE *file = fopen("file.dat", "wt");

    int w = fwrite(arr, sizeof(float), nx * ny, file);
    std::cout << "writen: " << w << std::endl;
    fclose(file);
}

void calcVectorStep(){

}

float calculateWave(float *u, float *uNext, float *p, int nx, int ny, float tau, int n, float hx, float hy, int beginImpulseX, int beginImpulseY) {
    float uMax = 0;
    float fValue = f(1.0, n, tau, 1.5, 4.0);
    float hxSquare = hx * hx;
    float hySquare = hy * hy;
    float tauSquare = tau * tau;

    float tauSquareArr[VECTOR_SIZE] = {tauSquare, tauSquare, tauSquare, tauSquare,tauSquare, tauSquare, tauSquare, tauSquare};
    float twoFloatArr[VECTOR_SIZE] = {2.0, 2.0, 2.0, 2.0,2.0, 2.0, 2.0, 2.0};
    float impulseArr[VECTOR_SIZE];// = {0, 0, 0, 0};

    float hxSquareArr[VECTOR_SIZE] = {hxSquare, hxSquare, hxSquare, hxSquare,hxSquare, hxSquare, hxSquare, hxSquare};
    float hySquareArr[VECTOR_SIZE] = {hySquare, hySquare, hySquare, hySquare,hySquare, hySquare, hySquare, hySquare};

    float impulsePosValue = 0;

    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j += VECTOR_SIZE) {
            if (i == beginImpulseX && (beginImpulseY - j) < VECTOR_SIZE && (beginImpulseY - j) >=0) {
                impulseArr[(beginImpulseY - j)] = fValue;
            } else {
                memset(impulseArr, 0, 8 * sizeof(float));
            }

            __m256 first;
            __m256 second;
            __m256 firstPath;
            __m256 secondPath;
            __m256 res1;
            __m256 res2;
            __m256 lineDividend;
            __m256 firstLineResult;
            __m256 secondLineResult;
            __m256 divider;
            __m256 sumOfTwoLines;
            __m256 bracketResult;
            __m256 impulseVector;
            __m256 tauSquareVector;
            __m256 mulResult;
            __m256 totalResult;

//(3) get(u, nx, i, j+1) - get(u, nx, i, j)
            first = _mm256_loadu_ps(getV(u, nx, i, j + 1));
            second = _mm256_loadu_ps(getV(u, nx, i, j));
            res1 = _mm256_sub_ps(first, second);

//(3) get(p, nx, i-1, j) + get(p, nx, i, j)
            first = _mm256_loadu_ps(getV(p, nx, i - 1, j));
            second = _mm256_loadu_ps(getV(p, nx, i, j));
            res2 = _mm256_add_ps(first, second);

//(3) ((get(u, nx, i, j+1) - get(u, nx, i, j)) * (get(p, nx, i-1, j) + get(p, nx, i, j))
            firstPath = _mm256_mul_ps(res1, res2);

//(3) get(u, nx,i, j-1) - get(u, nx, i, j)
            first = _mm256_loadu_ps(getV(u, nx, i, j - 1));
            second = _mm256_loadu_ps(getV(u, nx, i, j));
            res1 = _mm256_sub_ps(first, second);

//(3) get(p, nx,i-1,j-1) + get(p, nx, i,j-1)
            first = _mm256_loadu_ps(getV(p, nx, i - 1, j - 1));
            second = _mm256_loadu_ps(getV(p, nx, i, j - 1));
            res2 = _mm256_add_ps(first, second);

//(3) ((get(u, nx,i, j-1) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx, i,j-1)))
            secondPath = _mm256_mul_ps(res1, res2);

//(3) ((((get(u, nx, i, j+1) - get(u, nx, i, j)) * (get(p, nx, i-1, j) + get(p, nx, i, j))) + ((get(u, nx,i, j-1) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx, i,j-1))))
            lineDividend = _mm256_add_ps(firstPath, secondPath);

//(3) (2 * hxSquare)
            first = _mm256_loadu_ps(twoFloatArr);
            second = _mm256_loadu_ps(hxSquareArr);
            divider = _mm256_mul_ps(first, second);
//(3) result
            firstLineResult = _mm256_div_ps(lineDividend, divider);

//////////////////////////////теперь 4 строка/////////////////////////////////////////
//(4) get(u, nx, i+1, j) - get(u, nx, i, j)
            first = _mm256_loadu_ps(getV(u, nx, i + 1, j));
            second = _mm256_loadu_ps(getV(u, nx, i, j));
            res1 = _mm256_sub_ps(first, second);

//(4) get(p, nx, i, j-1) + get(p, nx, i, j)
            first = _mm256_loadu_ps(getV(p, nx, i, j - 1));
            second = _mm256_loadu_ps(getV(p, nx, i, j));
            res2 = _mm256_add_ps(first, second);

//(4) ((get(u, nx, i, j+1) - get(u, nx, i, j)) * (get(p, nx, i-1, j) + get(p, nx, i, j))
            firstPath = _mm256_mul_ps(res1, res2);

//(4) get(u, nx, i-1,j) - get(u, nx, i, j)
            first = _mm256_loadu_ps(getV(u, nx, i - 1, j));
            second = _mm256_loadu_ps(getV(u, nx, i, j));
            res1 = _mm256_sub_ps(first, second);

//(4) get(p, nx,i-1,j-1) + get(p, nx,i-1, j)
            first = _mm256_loadu_ps(getV(p, nx, i - 1, j - 1));
            second = _mm256_loadu_ps(getV(p, nx, i - 1, j));
            res2 = _mm256_add_ps(first, second);

//(4) ((get(u, nx, i-1,j) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx,i-1, j))
            secondPath = _mm256_mul_ps(res1, res2);

//(4) ((((get(u, nx, i+1, j) - get(u, nx, i, j)) * (get(p, nx, i, j-1) + get(p, nx, i, j))) + ((get(u, nx, i-1,j) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx,i-1, j))))
            lineDividend = _mm256_add_ps(firstPath, secondPath);

//(4) (2 * hySquare)
            first = _mm256_loadu_ps(twoFloatArr);
            second = _mm256_loadu_ps(hySquareArr);
            divider = _mm256_mul_ps(first, second);
//(4) result
            secondLineResult = _mm256_div_ps(lineDividend, divider);

//(3)+(4)
            sumOfTwoLines = _mm256_add_ps(firstLineResult, secondLineResult);

//impulsePosValue + (3,4)
            impulseVector = _mm256_loadu_ps(impulseArr);
            bracketResult = _mm256_add_ps(sumOfTwoLines, impulseVector);

//(1) 2 * get(u, nx, i, j)
            first = _mm256_loadu_ps(twoFloatArr);
            second = _mm256_loadu_ps(getV(u, nx, i, j));
            totalResult = _mm256_mul_ps(first, second);

//(1) prev - get(uNext, nx, i, j)
            second = _mm256_loadu_ps(getV(uNext, nx, i, j));
            totalResult = _mm256_sub_ps(totalResult, second);

//(1) tauSquare * ((2,3,4))
            tauSquareVector = _mm256_loadu_ps(tauSquareArr);
            mulResult = _mm256_mul_ps(tauSquareVector, bracketResult);

//(result)
            totalResult = _mm256_add_ps(totalResult, mulResult);
            _mm256_storeu_ps(uNext + i * nx + j, totalResult);

/*
          (1)  *(uNext + i * nx + j) = 2 * get(u, nx, i, j) - get(uNext, nx, i, j) + tauSquare * (
          (2)        impulsePosValue
          (3)        + ((((get(u, nx, i, j+1) - get(u, nx, i, j)) * (get(p, nx, i-1, j) + get(p, nx, i, j))) + ((get(u, nx,i, j-1) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx, i,j-1)))) / (2 * hxSquare))
          (4)        + ((((get(u, nx, i+1, j) - get(u, nx, i, j)) * (get(p, nx, i, j-1) + get(p, nx, i, j))) + ((get(u, nx, i-1,j) - get(u, nx, i, j)) * (get(p, nx,i-1,j-1) + get(p, nx,i-1, j)))) / (2 * hySquare))
            );
*/
        }
    }

    return uMax;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "<n> - шагов по времени\n"
                     "<nx> - размер сети по оси x (кратно 4)\n"
                     "<ny> - размер сети по оси y (кратно 4)" << std::endl;
        return 0;
    }

    int n;
    int nx;
    int ny;

    try {
        n = std::stoi(argv[1]);
        nx = std::stoi(argv[2]);
        ny = std::stoi(argv[3]);

        if (nx % VECTOR_SIZE != 0 || ny % VECTOR_SIZE != 0)
            throw std::invalid_argument("error");
    } catch (std::invalid_argument &e) {
        std::cout << "лучше ввести int кратный " << VECTOR_SIZE << " для <nx> и <ny>" << std::endl;
        return 0;
    }

    float xa = 0.0;
    float xb = 4.0;
    float ya = 0.0;
    float yb = 4.0;

    float hx = (xb - xa) / (nx - 1);
    float hy = (yb - ya) / (ny - 1);

    float tau;
    if (nx <= 1000 && ny <= 1000)
        tau = 0.01;
    else
        tau = 0.001;

    float *u = (float *) std::aligned_alloc(ALIGN_INDENT, nx * ny * sizeof(float));
    float *uNext = (float *) std::aligned_alloc(ALIGN_INDENT, nx * ny * sizeof(float));

    for (int i = 0; i < (nx * ny); i++) {
        *(u + i) = 0;
        *(uNext + i) = 0;
    }

    float *p = (float *) std::aligned_alloc(ALIGN_INDENT, nx * ny * sizeof(float));
    insertValuesInP(p, nx, ny);

    int beginImpulseX = 1;
    int beginImpulseY = 1;
    set(uNext, nx, beginImpulseX, beginImpulseY, 10);

    for (int i = 0; i < n; i++) {
        calculateWave(u, uNext, p, nx, ny, tau, i, hx, hy, beginImpulseX, beginImpulseY);
        swapPtr(&u, &uNext);
    }

//    printArrayInFile(u, nx, ny);

    std::free(uNext);
    std::free(u);
    std::free(p);
    return 0;
}
