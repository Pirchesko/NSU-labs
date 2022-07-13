package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import org.springframework.stereotype.Component;

import java.util.Arrays;

@Component
public class Echo implements Algorithm {
    @Override
    public Result calculate(Input input) {
        String[] answer = new String[6];
        answer[0] = "Вы отправили эхо запрос с параметрами:";
        answer[1] = "action:" + input.action;
        answer[2] = "n:" + input.n;
        answer[3] = "g:" + Arrays.toString(input.g);
        answer[4] = "q:" + input.q;
        answer[5] = "u:" + Arrays.toString(input.u);;
        return new Result(true, answer);
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        return null;
    }
}
