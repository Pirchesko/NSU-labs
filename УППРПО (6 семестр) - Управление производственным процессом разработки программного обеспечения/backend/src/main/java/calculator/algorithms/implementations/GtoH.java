package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import org.springframework.stereotype.Component;

import java.util.Arrays;

@Component
public class GtoH implements Algorithm {
    private int baseStart;
    private Matrix matrix;
    private Matrix newMatrix;
    private int[] perestanovki;
    String msg = "";

    @Override
    public Result calculate(Input input) {
        msg = "";
        Matrix newMatrix = toGorH(input.matrix);
        if (msg != ""){
            return new Result(false, msg);
        } else {
            return new Result(true, newMatrix.toStringArray());
        }
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.matrix = true;
        return whatToValidate;
    }

    Matrix toGorH(Matrix inputMatrix){
        matrix = inputMatrix;
        if (matrix.getLines() >= matrix.getColumns()){
            msg = "Количество строк больше или равно количеству столбиков - некорректные входные данные";
            return inputMatrix;
        }
        System.out.println("Input matrix (Lines = " + matrix.getLines() + ", Columns = " + matrix.getColumns() + ")");
        printMatrix(matrix);

        matrix = matrix.toModule(matrix);

        perestanovki = new int[matrix.getColumns()];
        Arrays.fill(perestanovki, -1);
        int count_stolbikov = checkPerestanovka();
        if (count_stolbikov == -1){
            return inputMatrix;
        }

        newMatrix = new Matrix(matrix.getColumns() - matrix.getLines(), matrix.getColumns(), matrix.getQ(), 0);
        changeMatrix();
        System.out.println("Matrix before perestanovki:");
        printMatrix(newMatrix);
        if (count_stolbikov > 0) {
            changeStolbiki(count_stolbikov);
        }

        System.out.println("Complete change matrix:");
        printMatrix(newMatrix);
        System.out.println();

        return newMatrix;
    }

    void printMatrix(Matrix M){
        for (int i = 0; i < M.getLines(); i++) {
            for (int j = 0; j < M.getColumns(); j++) {
                System.out.print(M.data[i][j] + " ");
            }
            System.out.println();
        }
    }

    //Проверяем нужны ли перестановки столбцов, и если да, то меняем из местами
    int checkPerestanovka(){
        int[] check = new int[matrix.getColumns()];
        Arrays.fill(check, -1);
        for (int i = 0; i < matrix.getColumns(); i++){
            int count_0 = 0;
            int count_1 = 0;
            int last_pos_1 = -1;
            for (int j = 0; j < matrix.getLines(); j++) {
                if (matrix.data[j][i] == 0) {
                    count_0++;
                }
                if (matrix.data[j][i] == 1) {
                    count_1++;
                    last_pos_1 = j;
                }
            }
            if ((count_0 == matrix.getLines() - 1) && (count_1 == 1)) {
                check[i] = last_pos_1;
            }
        }

//        System.out.println("Before perestanovki:");
//        for (int i = 0; i < matrix.getQ(); i++) {
//            System.out.print(check[i] + " ");
//        }

        int left_value = 0; //выбираем, слева или справа будем помещать единичную матрицу
        int right_value = 0;
        for (int i = 0; i < matrix.getLines(); i++) {
            if (check[i] == i) {
                left_value++;
            }
        }
        for (int i = matrix.getColumns() - matrix.getLines(); i < matrix.getColumns(); i++) {
            if (check[i] == i - (matrix.getColumns() - matrix.getLines())) {
                right_value++;
            }
        }

        System.out.println("Values of left = " + left_value + "; right = " + right_value);
        int baseFinish;
        if (left_value > right_value) {
            baseStart = 0;
            baseFinish = matrix.getLines();
        }
        else {
            baseStart = matrix.getColumns() - matrix.getLines();
            baseFinish = matrix.getColumns();
        }

        int count_stolbiki = 0; //делаем перестановки при необходимости в массиве для перестановок
        for (int i = baseStart; i < baseFinish; i++) {
            if (check[i] != i - baseStart) {
                for (int j = 0; j < matrix.getColumns(); j++) {
                    if (check[j] == i - baseStart) {
                        System.out.println("Change elements: " + i + " and " + j);
                        int tmp = check[j];
                        check[j] = check[i];
                        check[i] = tmp;
                        perestanovki[count_stolbiki] = i;
                        perestanovki[count_stolbiki + 1] = j;
                        count_stolbiki += 2;
                        break;
                    }
                }
            }
        }

        System.out.println("After perestanovki");
        for (int i = 0; i < matrix.getColumns(); i++) {
            System.out.print(check[i] + " ");
        }
        System.out.println();

        int[] check_E = new int[matrix.getLines()];
        Arrays.fill(check_E, 0);
        for(int i = 0; i < check.length; i++){
            if (check[i] != -1)
            {
                check_E[check[i]]++;
            }
        }
        for (int i = 0; i < check_E.length; i++){
            if (check_E[i] < 1){
                System.out.println("Cannot convert this matrix");
                msg = "Алгоритм не может конвертировать матрицу";
                return -1;
            }
        }

        if (count_stolbiki > 0) { //теперь делаем перестановки в самой матрице
            for (int k = 0; k < count_stolbiki; k += 2) {
                for (int i = 0; i < matrix.getLines(); i++) {
                    int tmp = matrix.data[i][perestanovki[k]];
                    matrix.data[i][perestanovki[k]] = matrix.data[i][perestanovki[k + 1]];
                    matrix.data[i][perestanovki[k + 1]] = tmp;
                }
            }
        }

        System.out.println("Matrix in standart view");
        printMatrix(matrix);
        return count_stolbiki;
    }

    //Выполенение траспонирования матрицы
    void changeMatrix(){
        if (baseStart == 0) { //единичная матрица слева
            for (int i = 0; i < matrix.getLines(); i++) { //ставим матрицу A
                for (int j = matrix.getLines(); j < matrix.getColumns(); j++) {
                    newMatrix.data[j - matrix.getLines()][i] = -1 * matrix.data[i][j];
                }
            }
            for (int i = matrix.getLines(); i < matrix.getColumns(); i++) { //ставим единички для E
                newMatrix.data[i - matrix.getLines()][i] = 1;
            }
        }
        else {
            for (int i = 0; i < matrix.getLines(); i++) {
                for (int j = 0; j < matrix.getColumns() - matrix.getLines(); j++) {
                    newMatrix.data[j][i + matrix.getColumns() - matrix.getLines()] = -1 * matrix.data[i][j];
                }
            }
            for (int i = 0; i < matrix.getColumns() - matrix.getLines(); i++) {
                newMatrix.data[i][i] = 1;
            }
        }

        System.out.println("Transporting...");
        printMatrix(newMatrix);
        newMatrix = matrix.toModule(newMatrix);
    }

    void changeStolbiki(int count_stolbikov){
        for (int k = count_stolbikov - 2; k >= 0; k -= 2) {
            for (int i = 0; i < newMatrix.getLines(); i++) {
                int tmp = newMatrix.data[i][perestanovki[k]];
                newMatrix.data[i][perestanovki[k]] = newMatrix.data[i][perestanovki[k + 1]];
                newMatrix.data[i][perestanovki[k + 1]] = tmp;
            }
        }
    }

    void division(int[] dividend, int[] divisor, int[] quotient, int[] remainder)
    {
        /*if (dividend.Last() == 0)
        {
            throw new ArithmeticException("Старший член многочлена делимого не может быть 0");
        }
        if (divisor.Last() == 0)
        {
            throw new ArithmeticException("Старший член многочлена делителя не может быть 0");
        }*/
        //remainder = (double[])dividend.Clone();
        remainder = new int[dividend.length];
        for (int i = 0; i < dividend.length; i++){
            remainder[i] = dividend[i];
        }
        quotient = new int[remainder.length - divisor.length + 1];
        for (int i = 0; i < quotient.length; i++)
        {
            int coeff = remainder[remainder.length - i - 1] / divisor[divisor.length - 1]; //divisor.Last();
            quotient[quotient.length - i - 1] = coeff;
            for (int j = 0; j < divisor.length; j++)
            {
                remainder[remainder.length - i - j - 1] -= coeff * divisor[divisor.length - j - 1];
            }
        }
    }
}
