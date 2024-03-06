#include <iostream>
#include <Windows.h>

//Функия u(0,x)=v(x)
double v(double x) //нужна точность через abs?
{
    if (x >= 0)
    {
        return 0.0;
    }
    else
    {
        return 1.0;
    }
}

//Функция u(t,x)
double u(double x, int j, int t, double a, double b, int N, double C, double r, double* setka)
{
    /*if (j == 0)
    {
        return v(a);
    }
    else if (j == N)
    {
        return v(b);
    } 
    else if (t == 0)
    {
        return v(x);
    }*/
    if (t == 0)
    {
        return v(x);
    } 
    else if (j == 0)
    {
        return v(a);
    }
    else if (j == N)
    {
        //return v(b);
        return 0.0;
    }
    else //u^(n+1)_j = (C*r) * (u^n_(j+1) - 2*u^n_j + u^n_(j-1)) - (r/2) * (u^n_(j+1) - u^n_(j-1)) + u^n_j
    {
        //return C * r * (setka[(t - 1) * N + j + 1] - 2 * setka[(t - 1) * N + j] + setka[(t - 1) * N + j - 1]) - (r / 2.0) * (setka[(t - 1) * N + j + 1] - setka[(t - 1) * N + j - 1]) + setka[(t - 1) * N + j];
        return C * r * (setka[(t - 1) * N + j + 1] - 2 * setka[(t - 1) * N + j] + setka[(t - 1) * N + j - 1]) - (r / 2.0) * (setka[(t - 1) * N + j + 1] - setka[(t - 1) * N + j - 1]) + setka[(t - 1) * N + j];
    }
}

//Эта штука чертит жирную точку (Ellips - не хочет работать)
void drawPoint(HDC hDC, int x, int y, int R, int G, int B)
{
    SetPixel(hDC, x, y, RGB(R, G, B));
    SetPixel(hDC, x - 1, y, RGB(R, G, B));
    SetPixel(hDC, x - 2, y, RGB(R, G, B));
    SetPixel(hDC, x + 1, y, RGB(R, G, B));
    SetPixel(hDC, x + 2, y, RGB(R, G, B));
    SetPixel(hDC, x, y - 1, RGB(R, G, B));
    SetPixel(hDC, x - 1, y - 1, RGB(R, G, B));
    SetPixel(hDC, x - 2, y - 1, RGB(R, G, B));
    SetPixel(hDC, x + 1, y - 1, RGB(R, G, B));
    SetPixel(hDC, x + 2, y - 1, RGB(R, G, B));
    SetPixel(hDC, x, y + 1, RGB(R, G, B));
    SetPixel(hDC, x - 1, y + 1, RGB(R, G, B));
    SetPixel(hDC, x - 2, y + 1, RGB(R, G, B));
    SetPixel(hDC, x + 1, y + 1, RGB(R, G, B));
    SetPixel(hDC, x + 2, y + 1, RGB(R, G, B));
    SetPixel(hDC, x, y - 2, RGB(R, G, B));
    SetPixel(hDC, x - 1, y - 2, RGB(R, G, B));
    SetPixel(hDC, x + 1, y - 2, RGB(R, G, B));
    SetPixel(hDC, x, y + 2, RGB(R, G, B));
    SetPixel(hDC, x - 1, y + 2, RGB(R, G, B));
    SetPixel(hDC, x + 1, y + 2, RGB(R, G, B));
}

int main()
{
    double a = -1.0;
    double b = 10.0;
    double h = 0.1;
    double tau = 0.01;
    double r = tau / h;
    double x;
    double c1 = 1.0 / 2.0;
    double c2 = 1.0 / (2.0 * r);
    double c3 = 1.0 / 10.0;
    int N = (b - a) / h; //Количество шагов по x
    int T = 500; //количество временных шагов
    double* setka = (double*)malloc((N + 1) * (T + 1) * sizeof(double)); //точное решение
    double* setka1 = (double*)malloc((N + 1) * (T + 1) * sizeof(double)); //решение для C1
    double* setka2 = (double*)malloc((N + 1) * (T + 1) * sizeof(double)); //решение для C2
    double* setka3 = (double*)malloc((N + 1) * (T + 1) * sizeof(double)); //решение для C3

    //Несколько необходимых данных для вычисления, где следует сделать паузу в программе
    double top_red = -9.0;
    double top_min = 10.0;
    int top_t = 99999;
    //Сначала заранее просчитаем массивы данных (а потом по ним будем рисовать)
    for (int t = 0; t <= T; t++) //Начинаем идти по временным слоям (т.е. идём по всем n)
    {
        double x_ = 1.0; //точное решение - вертикальная его линия
        //Рассчитываем на текущем временном слое значения в каждом x (т.е. высчитываем каждый j-ый)
        for (int j = 0; j <= N; j++)
        {
            x = a + j * h;
            setka[t * N + j] = v(x - t * tau); //Точное решение было изменено! v(x-t) -> v(x-t*tau) - так просто логичнее     
            setka1[t * N + j] = u(x, j, t, a, b, N, c1, r, setka1);
            setka2[t * N + j] = u(x, j, t, a, b, N, c2, r, setka2);
            setka3[t * N + j] = u(x, j, t, a, b, N, c3, r, setka3);
            /*if ((j == 110) || (j == 109) || (j == 108))
            {
                printf("setka_: t = %d, j = %d, x = %f, u = %f\n ", t, j, x, setka[t * N + j]);
                printf("setka1: t = %d, j = %d, x = %f, u = %f\n ", t, j, x, setka1[t * N + j]);
                printf("setka2: t = %d, j = %d, x = %f, u = %f\n ", t, j, x, setka2[t * N + j]);
                printf("setka3: t = %d, j = %d, x = %f, u = %f\n ", t, j, x, setka3[t * N + j]);
            }*/
                
            if ((j > 0) && (setka[t * N + j] == 0.0) && (setka[t * N + j - 1] == 1.0))
            {
                x_ = x; //просчитываем, где у нас вертикальная красная линия точного решения
            }
        }
        if (t == 500)
        {
            std::cin.get();
        }
        
        double abs12, abs13, abs23; //Разница между значениями соотвествующих графиков
        for (int j = 0; j <= N; j++) //Расчитываем при каком t все точки графиков в близи точного решения, находятся максимально близко - в одной точке
        {
            x = a + j * h;
            abs12 = abs(setka1[t * N + j] - setka2[t * N + j]);
            abs13 = abs(setka1[t * N + j] - setka3[t * N + j]);
            abs23 = abs(setka2[t * N + j] - setka3[t * N + j]);
            if ((x >= x_ - 0.4) && (x <= x_ - 0.1)) //считаем недалеко от вертикальной красной линиии
            {
                if ((abs12 + abs13 + abs23 < top_min) && (x_ > 2.0))
                {
                    top_min = abs12 + abs13 + abs23;
                    top_red = x_;
                    top_t = t;
                }
            }
        }
    }
    //printf("top_red = %f\n top_abs = %f\n top_t = %d", top_red, top_min, top_t);

    //Инициализация рисовалки
    HWND hWnd = GetConsoleWindow(); //Берём текущее окно
    //Получаем дескриптор для рисования: 
    HDC hDC = GetDC(hWnd); //для осей X и Y на графике
    HDC hDC0 = GetDC(hWnd); //для точного решения
    HDC hDC1 = GetDC(hWnd); //для C1
    HDC hDC2 = GetDC(hWnd); //для C2
    HDC hDC3 = GetDC(hWnd); //для C3
    //Задаем то, чем рисуем
    HPEN penXY = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); //для осей X и Y на графике
    HPEN penGraph0 = CreatePen(PS_SOLID, 1, RGB(255, 128, 128)); //для точного решения
    HPEN penGraph1 = CreatePen(PS_SOLID, 1, RGB(160, 255, 255)); //для C1
    HPEN penGraph2 = CreatePen(PS_SOLID, 1, RGB(255, 255, 160)); //для C2
    HPEN penGraph3 = CreatePen(PS_SOLID, 1, RGB(255, 160, 255)); //для C3
    int s_y = 200; //Ось ординат Y
    int s_x = 600; //Ось абцисс X

    printf("Open on all window and press Enter\n");
    std::cin.get();
    //Начинаем идти по временным слоям (т.е. идём по всем n)
    for (int t = 0; t <= T; t++)
    {
        //Вывод вобщей информации
        printf("[a;b] = [%f; %f]\t", a, b); 
        printf("N = %d\n", N);
        printf("h = %f\t", h); 
        printf("tau = %f\t", tau); 
        printf("r = tau/h = %f\n", r);
        printf("c1 = %f\t", c1);
        printf("c2 = %f\t", c2);
        printf("c3 = %f\n", c3);
        printf("t = %d\t", t);

        //Чертим оси для удобства белым цветом
        SelectObject(hDC, penXY);
        MoveToEx(hDC, s_y, 0, NULL);
        LineTo(hDC, s_y, 1000);
        MoveToEx(hDC, 0, s_x, NULL);
        LineTo(hDC, 1800, s_x);
        
        //А теперь рисуем
        for (int i = 0; i <= N; i++) //Сначала чертим график (соединяем точки - линиями)
        {
            if (i == 0) //Для самых певрых точек перемещение в стартовую позицию
            {
                SelectObject(hDC0, penGraph0);
                MoveToEx(hDC0, s_y - 150, s_x - (int)(setka[t * N] * 400.0), NULL);
                
                SelectObject(hDC1, penGraph1);
                MoveToEx(hDC1, s_y - 150, s_x - (int)(setka1[t * N] * 400.0), NULL);
                
                SelectObject(hDC2, penGraph2);
                MoveToEx(hDC2, s_y - 150, s_x - (int)(setka2[t * N] * 400.0), NULL);
                
                SelectObject(hDC3, penGraph3);
                MoveToEx(hDC3, s_y - 150, s_x - (int)(setka3[t * N] * 400.0), NULL);
                
            }
            else //Тут уже чертим график
            {
                SelectObject(hDC0, penGraph0);
                LineTo(hDC0, s_y - 150 + i * 15, s_x - (int)(setka[t * N + i - 1] * 400.0));
                LineTo(hDC0, s_y - 150 + i * 15, s_x - (int)(setka[t * N + i] * 400.0));

                SelectObject(hDC1, penGraph1);
                LineTo(hDC1, s_y - 150 + i * 15, s_x - (int)(setka1[t * N + i] * 400.0));
                
                SelectObject(hDC2, penGraph2);
                LineTo(hDC2, s_y - 150 + i * 15, s_x - (int)(setka2[t * N + i] * 400.0));
                
                SelectObject(hDC3, penGraph3);
                LineTo(hDC3, s_y - 150 + i * 15, s_x - (int)(setka3[t * N + i] * 400.0));
            }
        }
        for (int i = 0; i <= N; i++) //Затем точки делаем круглыми, чтобы их было видно (поверх нарисованного графика)
        {
            SelectObject(hDC0, penGraph0);
            drawPoint(hDC0, s_y - 150 + i * 15, s_x - (int)(setka[t * N + i] * 400.0), 255, 0, 0);
            SetPixel(hDC0, s_y - 150 + i * 15, s_x - (int)(setka[t * N + i] * 400.0), RGB(255, 255, 255)); //в самой середике оставим 1 пиксель белым

            SelectObject(hDC1, penGraph1);
            drawPoint(hDC1, s_y - 150 + i * 15, s_x - (int)(setka1[t * N + i] * 400.0), 0, 255, 255);
            SetPixel(hDC1, s_y - 150 + i * 15, s_x - (int)(setka1[t * N + i] * 400.0), RGB(255, 255, 255));

            SelectObject(hDC2, penGraph2);
            drawPoint(hDC2, s_y - 150 + i * 15, s_x - (int)(setka2[t * N + i] * 400.0), 255, 255, 0);
            SetPixel(hDC2, s_y - 150 + i * 15, s_x - (int)(setka2[t * N + i] * 400.0), RGB(255, 255, 255));

            SelectObject(hDC3, penGraph3);
            drawPoint(hDC3, s_y - 150 + i * 15, s_x - (int)(setka3[t * N + i] * 400.0), 255, 0, 255);
            SetPixel(hDC3, s_y - 150 + i * 15, s_x - (int)(setka3[t * N + i] * 400.0), RGB(255, 255, 255));
        }

        if (t == top_t)
        {
            printf("sum_abs = %f\n ", top_min);
            printf("Press enter to resume...");
            std::cin.get();
        }

        //std::cin.get();

        Sleep(10); //задержка для отображение точек
        static const char* CSI = "\33["; //для очистки экрана
        printf("%s%c%s%c", CSI, 'H', CSI, '2J'); //чистим
        //system("cls"); //тоже чистилка экрана, но от неё сильно мерцает
    }
    
    //Освобождаем ресурсы
    DeleteObject(penXY);
    DeleteObject(penGraph0);
    DeleteObject(penGraph1);
    DeleteObject(penGraph2);
    DeleteObject(penGraph3);
    ReleaseDC(hWnd, hDC);
    ReleaseDC(hWnd, hDC0);
    ReleaseDC(hWnd, hDC1);
    ReleaseDC(hWnd, hDC2);
    ReleaseDC(hWnd, hDC3);

    return 0;
}

    /*
    // Получаем дескрипторы для рисования
    HWND hWnd = GetConsoleWindow();
    HDC hDC = GetDC(hWnd);
    // задаем то, чем рисуем
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    SelectObject(hDC, hPen);
    // рисуем линию из позиции (100, 100) в (200, 200), а потом в (300, 200)
    MoveToEx(hDC, 100, 100, NULL);
    LineTo(hDC, 200, 200);
    LineTo(hDC, 300, 200);
    std::cin.get();

    // При завершении работы необходимо освободить ресурсы
    DeleteObject(hPen);
    ReleaseDC(hWnd, hDC);
    */