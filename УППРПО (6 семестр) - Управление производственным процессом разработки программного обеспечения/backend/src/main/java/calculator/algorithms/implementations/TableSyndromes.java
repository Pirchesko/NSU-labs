package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

@Component
public class TableSyndromes implements Algorithm {
    @Override
    public Result calculate(Input input) {
        String[] result = new String[input.matrix.getColumns()];
        Vector vector = new Vector(input.q, input.matrix.getColumns());
        vector.data[0] = 1;
        for (int i = 0; i < input.matrix.getColumns(); i++) {
            if (i > 1)
                vector.data[i-1] = 0;
            vector.data[i] = 1;

            StringBuilder str = new StringBuilder();
            str.append(vector);
            str.append(": ");

            Vector syndrome = input.matrix.mulToVec(vector.data);
            str.append(syndrome.toString());
            result[i] = str.toString();
            vector.data[i] = 0;
            if (i+1 < vector.data.length)
                vector.data[i+1] = 1;
        }

        return new Result(true, result);
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.matrix = true;
        return whatToValidate;
    }
}
