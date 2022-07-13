package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

import java.util.Arrays;

import static calculator.model.Vector.divisionPolynom;

@Component
public class FirstSystematic implements Algorithm {
    @Override
    public Result calculate(Input input) {
        int n = input.n;
        int q = input.q;
        int[] g_ = new int[input.g.length];
        System.arraycopy(input.g, 0, g_, 0, input.g.length);
        Vector g = new Vector(g_, q);
        int r = g.data.length - 1;

        if (r == 0)
            return new Result(false, "g не может быть степени 0");
        if (g.data[g.data.length - 1] == 0)
            return new Result(false, "g не может оканчиваться на 0");

        int k = n - r;
        Matrix firstMatrix = new Matrix(k, n, q, 0); //матрица первого систематического кодера
        for (int i = r; i <= n - 1; i++) {
            Vector F = new Vector(q, i + 1);
            F.data[F.data.length - 1] = 1;
            Vector quotient = new Vector(q);
            Vector remainder = new Vector(q);
            divisionPolynom(F, g, quotient, remainder);
            //System.out.println(remainder.data.toString());
            for (int j = 0; j < remainder.data.length; j++) {
                firstMatrix.data[i - r][j] = remainder.data[j];
            }
        }

        for (int i = 0; i < firstMatrix.getLines(); i++){
            firstMatrix.data[i][n-k+i] = 1;
        }
        return new Result(true, firstMatrix.toStringArray());
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.g = true;
        whatToValidate.n = true;
        return whatToValidate;
    }
}
