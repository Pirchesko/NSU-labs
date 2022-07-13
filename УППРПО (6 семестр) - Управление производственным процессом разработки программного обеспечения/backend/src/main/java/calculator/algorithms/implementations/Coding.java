package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

@Component
public class Coding implements Algorithm {
    @Override
    public Result calculate(Input input) {
        if (input.matrix.getLines() != input.u.length)
            return new Result(false, "Длина вектора u должна быть равна числу строк матрицы");

        Matrix result = new Matrix(1, input.matrix.getColumns());
        Vector v = new Vector(input.u, input.matrix.getQ());
        Vector temp = v.mulToMatrix(input.matrix);
        for (int i = 0; i < input.matrix.getColumns(); i++) {
            result.data[0][i] = temp.data[i];
        }

        return new Result(true, result.toString());
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.matrix = true;
        whatToValidate.u = true;
        return whatToValidate;
    }
}
