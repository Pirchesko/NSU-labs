package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import org.springframework.stereotype.Component;

@Component
public class HtoG implements Algorithm {
    @Override
    public Result calculate(Input input) {
        GtoH gtoH = new GtoH();
        Matrix newMatrix = gtoH.toGorH(input.matrix);
        if (gtoH.msg != ""){
            return new Result(false, gtoH.msg);
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
}
