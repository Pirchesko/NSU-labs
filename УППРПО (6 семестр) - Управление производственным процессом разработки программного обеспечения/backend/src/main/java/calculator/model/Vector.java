package calculator.model;

import java.util.Arrays;

public class Vector {
    public int[] data;
    private int q;

    public Vector(int q) {
        this.data = null;
        this.q = q;
    }


    public Vector(int[] data, int q) {
        this.data = data;
        this.q = q;
    }

    public Vector(int q, int size){
        this.data = new int[size];
        Arrays.fill(this.data, 0);
        this.q = q;
    }

    public Vector cloneData(Vector copied){
        this.data = new int[copied.data.length];
        this.q = copied.q;
        for (int i = 0; i < copied.data.length; i++){
            this.data[i] = copied.data[i];
        }
        return this;
    }

    public static Vector init(Vector input, int q, int size){
        input.data = new int[size];
        Arrays.fill(input.data, 0);
        input.q = q;
        return input;
    }

    public int getQ() {
        return q;
    }

    public void setQ(int q) {
        this.q = q;
    }

    public void increase(){
        for (int i = 1; i < data.length + 1; i++) {
            data[data.length-i] += 1;
            data[data.length-i] %= q;
            if (data[data.length-i] == 0){
                if (data.length-i == 0) break;
            } else break;
        }
    }

    public Vector mulToMatrix(Matrix matrixRight){
        Vector res = new Vector(new int[matrixRight.getColumns()], q);
        for (int col = 0; col < matrixRight.getColumns(); col++) {
            for (int i = 0; i < matrixRight.getLines(); i++) {
                res.data[col] += ( data[i] * matrixRight.data[i][col] ) % q;
            }
            res.data[col] %= q;
        }
        return res;
    }

    public String toString(){
        StringBuilder str = new StringBuilder();
        for (int datum : data) {
            str.append(datum);
            str.append(" ");
        }
        return str.toString();
    }

    static public Vector toModule(Vector vector){
        for (int i = 0; i < vector.data.length; i++) {
            if (vector.data[i] < 0) {
                while (vector.data[i] < 0) {
                    vector.data[i] += vector.q;
                }
            }
            else if (vector.data[i] >= vector.q) {
                vector.data[i] = vector.data[i] % vector.q;
            }
        }
        return vector;
    }

    public boolean isZero(){
        int sum = 0;
        for (int datum : data) {
            sum += datum;
        }
        return (sum == 0);
    }

    //dividend - делимое; divisor - делитель; quotient - частное; remainder - остаток
    static public void divisionPolynom(Vector dividend, Vector divisor, Vector quotient, Vector remainder)
    {
        if (dividend.data[dividend.data.length - 1] == 0) {
            System.out.println("Старший член многочлена делимого не может быть 0"); //у нас может
        }
        if (divisor.data[divisor.data.length - 1] == 0) {
            System.out.println("Старший член многочлена делителя не может быть 0");
        }
        remainder = remainder.cloneData(dividend);
        quotient = init(quotient, quotient.q, remainder.data.length - divisor.data.length + 1);
        for (int i = 0; i < quotient.data.length; i++)
        {
            int coeff = remainder.data[remainder.data.length - i - 1] / divisor.data[divisor.data.length - 1];
            quotient.data[quotient.data.length - i - 1] = coeff;
            for (int j = 0; j < divisor.data.length; j++)
            {
                remainder.data[remainder.data.length - i - j - 1] -= coeff * divisor.data[divisor.data.length - j - 1];
            }
        }
        remainder = toModule(remainder);
        quotient = toModule(quotient);
    }

    /*
    //На int[] - может пригодится
    public void divisionPolynom(int[] dividend, int[] divisor, int[] quotient, int[] remainder)
    {
        if (dividend[dividend.length - 1] == 0) {
            System.out.println("Старший член многочлена делимого не может быть 0");
        }
        if (divisor[divisor.length - 1] == 0) {
            System.out.println("Старший член многочлена делителя не может быть 0");
        }
        remainder = new int[dividend.length];
        for (int i = 0; i < dividend.length; i++) {
            remainder[i] = dividend[i];
        }
        quotient = new int[remainder.length - divisor.length + 1];
        for (int i = 0; i < quotient.length; i++)
        {
            int coeff = remainder[remainder.length - i - 1] / divisor[divisor.length - 1];
            quotient[quotient.length - i - 1] = coeff;
            for (int j = 0; j < divisor.length; j++)
            {
                remainder[remainder.length - i - j - 1] -= coeff * divisor[divisor.length - j - 1];
            }
        }

    }
     */
}
