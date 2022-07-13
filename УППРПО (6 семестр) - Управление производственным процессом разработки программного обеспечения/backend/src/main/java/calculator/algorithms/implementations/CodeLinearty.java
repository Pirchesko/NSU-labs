package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import org.springframework.stereotype.Component;

@Component
public class CodeLinearty implements Algorithm {
    @Override
    public Result calculate(Input input) {
        if (input.q != 2){
            return new Result(false, "Теорема Варшамова-Гилберта работает только для двоичных кодов, т.е. для q = 2");
        }
        if (input.d >= input.n) {
            return new Result(false, "Минимальное расстояние (d) должно быть меньше длины кода (n): d < n");
        }
        int sum = 0;
        for(int i = 0; i <= input.d - 2; i++) {
            sum+= getFactorial(input.n - 1)/(getFactorial(i)*getFactorial((input.n - 1) - i));
        }
        //int r = 2;
        int p = 1;
        while (sum - Math.pow(2, p) > 0) {
            p++;
        }
        if (input.k >= input.n - p){
            return new Result(true, "Такой линейный двоичный код существует [n, k, d]: [" + input.n + ", " + input.k + ", " + input.d + "]");
        }
        else {
            return new Result(false, "Не существует такого линейного двоичного кода с такими параметрами [n, k, d]: [" + input.n + ", " + input.k + ", " + input.d + "]");
        }

    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.d = true;
        whatToValidate.k = true;
        whatToValidate.n = true;
        return whatToValidate;
    }

    public static int getFactorial(int f) {
        int result = 1;
        for (int i = 1; i <= f; i++) {
            result = result * i;
        }
        return result;
    }
}
