#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//y(x_i) - точное (y=integrate g)
double Yx_i(double x_i)
{
    return (exp(x_i) * (cos(x_i) + sin(x_i))) / 2 - 0.5;
}

//g(x)=e^x*cos(x)
double g_i(double i)
{
    return exp(i)*cos(i);
}

//y_i=y_(i-1)+g_(i-1)*h
double scheme_k1(double i, double h, double a)
{
    if (a - i >= 0.000001)
    {
        cout << "Error in scheme_k1: " << i << " < " << a << endl;
    }
    else if (abs(i - a) < 0.000001)
    {
        return Yx_i(a);
        //return 0;
    }
    else
    {
        return g_i(i - h) * h + scheme_k1(i - h, h, a);
    }
    return 0;
}

//y_i=((g_i+g_(i-1))/2)*h+y_(i-1)
double scheme_k2(double i, double h, double a)
{
    if (a - i >= 0.000001)
    {
        cout << "Error in scheme_k2: " << i << " < " << a << endl;
        return 0;
    }
    else if (abs(i - a) < 0.000001)
    {
        return Yx_i(a);
        //return 0;
    }
    else
    {
        return ((g_i(i) + g_i(i - h)) / 2) * h + scheme_k2(i - h, h, a);
    }
}

//y_j=((5g_i+8g_(j-1)-g_(j-2))/12)*h+y_(j-1)
double scheme_k3(double i, double h, double a)
{
    if (a - i >= 0.000001)
    {
        cout << "Error in scheme_k4: " << i << " < " << a << endl;
        return 0;
    }
    else if (abs(i - a) < 0.000001)
    {
        return Yx_i(a);
        //return 0;
    }
    else
    {
        return ((5 * g_i(i - h) + 8 * g_i(i) - g_i(i + h)) / 12) * h + scheme_k3(i - h, h, a);
    }
}

//y_j=((g_j+4g_(j-1)+h_(j-2))/3)*h+y_(j-2)     y_j=((5g_i+8g_(j-1)-g_(j-2))/12)*h+y_(j-1)
double scheme_k4(double i, double h, double a)
{
    if (a - i >= 0.000001)
    {
        cout << "Error in scheme_k4: " << i << " < " << a << endl;
        return 0;
    }
    else if (abs(i - a) < 0.000001)
    {
        return Yx_i(a);
        //return 0;
    }
    else if (abs(i - (a + h)) < 0.000001)
    {
        return g_i(i - h) * h + scheme_k1(i - h, h, a);
        //return ((g_i(i) + g_i(i - h)) / 2) * h + scheme_k4(i - h, h, a);
        //return ((5 * g_i(i - h) + 8 * g_i(i) - g_i(i + h)) / 12) * h + scheme_k4(i - h, h, a);
    }
    else
    {
        return ((g_i(i) + 4 * g_i(i - h) + g_i(i - 2 * h)) / 3) * h + scheme_k4(i - 2 * h, h, a);
    }
}

double choose_scheme(double x_i, double h, double a, int k)
{
    switch (k)
        {
        case 1:
        {
            return scheme_k1(x_i, h, a);
            break;
        }
        case 2:
        {
            return scheme_k2(x_i, h, a);
            break;
        }
        case 3:
        {
            return scheme_k3(x_i, h, a);
            break;
        }
        case 4:
        {
            return scheme_k4(x_i, h, a);
            break;
        }
    }
}

void do_k(ofstream &file, double a, double b, double h, int k)
{
    double x_i;
    double y_i;
    double y_x_i;
    file << "k = " << k << "n" << endl;
    for (int i = 0; i <= (b - a) / h; i++)
    {
        file << i << ";";
        x_i = a + i * h;
        file << x_i << ";";
        y_i = choose_scheme(x_i, h, a, k);
        file << y_i << ";";
        y_x_i = Yx_i(x_i);
        file << y_x_i << ";";
        double ex1 = abs(y_i - y_x_i);
        file << ex1 << ";";
        y_i = choose_scheme(x_i, h / 3, a, k);
        y_x_i = Yx_i(x_i);
        double ex2 = abs(y_i - y_x_i);
        file << ex2 << ";";
        double p = log(ex1 / ex2) / log(3);//log2(ex1 / ex2); //log_a(b)=log_c(b)/log_c(a)
        if (ex2 == 0)
        {
            file << ";" << endl;
            file << "p = /0n" << endl;
        }
        else
        {
            file << ex1 / ex2 << ";" << endl;
            file << p << "n" << endl;
        }
    }
    /*
    int n = (b - a) / h;
    if (n % 2 == 0)
    {
        x_i = a + (b - a) / 2;
    }
    else
    {
        x_i = a + (b - a) / 2 - h / 2;
    }
    y_i = choose_scheme(x_i, h, a, k);
    y_x_i = Yx_i(x_i);
    double ex1 = abs(y_i - y_x_i);
    y_i = choose_scheme(x_i, h * 0.5, a, k);
    y_x_i = Yx_i(x_i);
    double ex2 = abs(y_i - y_x_i);
    double p = log2(ex1 / ex2);
    file << "p = " << p << "n" << endl;
    */
}

//n -> \n
void repair_csv()
{
    fstream file;
    string str;
    string output_file = "";
    file.open("D:\\Files\\Visual Studio\\Other\\Approximation\\Debug\\table.csv", ios::in);
    while (!file.eof())
    {
        file >> str;
        int i = 0;
        while (i < str.size())
        {
            if (str[i] == '.')
            {
                str[i] = ',';
            }
            if (str[i] == 'n')
            {
                str[i] = '\n';
            }
            i++;
        }
        output_file += str;
    }
    file.close();
    file.open("D:\\Files\\Visual Studio\\Other\\Approximation\\Debug\\table.csv", ios::out);
    file << output_file;
    file.close();
}

int main()
{
    ofstream file;
    file.open("D:\\Files\\Visual Studio\\Other\\Approximation\\Debug\\table.csv", ios::out);
    double h;
    double a;
    double b;
    cout << "h = ";
    cin >> h;
    cout << "a = ";
    cin >> a;
    cout << "b = ";
    cin >> b;
    if (a > b)
    {
        cout << "Error! [" << a << ";" << b << "] is bad. Retry!" << endl;
    }
    else
    {
        file << "i;x_i;y_i;y(x_i);er1(h)=|y_i-y(x)|;er2(h/3)=|y_i-y(x)|;ex1/ex2;porjadok n" << endl; //n -> \n
        do_k(file, a, b, h, 1);
        do_k(file, a, b, h, 2);
        do_k(file, a, b, h, 3);
        do_k(file, a, b, h, 4);
    }
    file.close();
    repair_csv();
}

