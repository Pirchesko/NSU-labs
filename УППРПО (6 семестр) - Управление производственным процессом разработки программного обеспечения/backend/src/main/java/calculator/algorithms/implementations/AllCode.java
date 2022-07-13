package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

import java.util.Arrays;

@Component
public class AllCode implements Algorithm {
    @Override
    public Result calculate(Input input) {
        int countVec = (int)Math.pow(input.matrix.getQ(), input.matrix.getLines());

        Vector v = new Vector(new int[input.matrix.getLines()],input.matrix.getQ());
        Arrays.fill(v.data, 0);

        String[] result = new String[countVec];

        for (int i = 0; i < countVec; i++) {
            StringBuilder str = new StringBuilder();
            Vector temp = v.mulToMatrix(input.matrix);
            for (int j = 0; j < v.data.length; j++) {
                str.append(v.data[j]);
                str.append(" ");
            }
            str.append("---> ");
            for (int j = 0; j < temp.data.length; j++) {
               str.append(temp.data[j]);
               str.append(" ");
            }
            v.increase();
            result[i] = str.toString();
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
