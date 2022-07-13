package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

import static calculator.model.Vector.divisionPolynom;

@Component
public class FindGfromH implements Algorithm {

    @Override
    public Result calculate(Input input) {
        int n = input.n;
        int q = input.q;
        int[] g_ = new int[input.g.length];
        System.arraycopy(input.g, 0, g_, 0, input.g.length);
        Vector g = new Vector(g_, q);

        Vector xn_1 = new Vector(q, n + 1);
        xn_1.data[0] = -1;
        xn_1.data[n] = 1;
        xn_1.toModule(xn_1);

        Vector quotient = new Vector(q);
        Vector remainder = new Vector(q);

        divisionPolynom(xn_1, g, quotient, remainder);

        for (int i = 0; i < remainder.data.length; i++){
            if (remainder.data[i] != 0){
                return new Result(false, "Деление получилось с остатком, проверьте входные данные");
            }
        }

        Matrix answer = new Matrix(1, quotient.data.length, 2, 0);
        for (int i = 0; i < quotient.data.length; i++){
            answer.data[0][i] = quotient.data[i];
        }
        return new Result(true, answer.toStringArray());
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.g = true;
        whatToValidate.n = true;
        return whatToValidate;
    }
}
