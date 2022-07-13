package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

import static calculator.model.Vector.divisionPolynom;

@Component
public class SecondSystematic implements Algorithm {
    @Override
    public Result calculate(Input input) {
        int n = input.n;
        int q = input.q;
        int[] g_ = new int[input.g.length];
        System.arraycopy(input.g, 0, g_, 0, input.g.length);
        Vector g = new Vector(g_, q);
        int[] u_ = new int[input.u.length];
        System.arraycopy(input.u, 0, u_, 0, input.u.length);
        Vector u = new Vector(u_, q);

        if (g.data[g.data.length - 1] == 0)
            return new Result(false, "Вектор g не может оканчиваться на 0");

        Matrix secondMatrix = new Matrix(1, n); // вектор длины n
        int r = n - u.data.length;
        Vector x_u = new Vector(q, n);
        for (int i = 0; i < u.data.length; i++){
            x_u.data[i + r] = u.data[i];
        }

        Vector quotient = new Vector(q);
        Vector remainder = new Vector(q);
        divisionPolynom(x_u, g, quotient, remainder);

        for (int j = 0; j < r; j++) {
            secondMatrix.data[0][j] = remainder.data[j];
        }
        for (int j = 0; j < u.data.length; j++){
            secondMatrix.data[0][r + j] = u.data[j];
        }

        return new Result(true, secondMatrix.toStringArray());
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.n = true;
        whatToValidate.g = true;
        whatToValidate.u = true;
        return whatToValidate;
    }
}
