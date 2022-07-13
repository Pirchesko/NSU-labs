package calculator.algorithms;

import calculator.jsondecs.Request;
import calculator.jsondecs.Response;

public class Validator {
    public static final class ValidateResult{
        public ValidateResult(boolean valid, String error) {
            this.valid = valid;
            this.error = error;
        }

        public boolean valid;
        public String error;
    }
    public static final class WhatToValidate{
        public WhatToValidate() {
            this.matrix = false;
            this.n = false;
            this.u = false;
            this.g = false;
            this.d = false;
            this.k = false;
        }
        public boolean matrix;
        public boolean n;
        public boolean u;
        public boolean g;
        public boolean d;
        public boolean k;
    }
    public static ValidateResult validate(Request request, WhatToValidate whatToValidate){
        if (whatToValidate == null)
            return new ValidateResult(true, "");

        if (request.getQ() == null) return new ValidateResult(false, "Вы не ввели q");
        if (request.getQ() < 2) return new ValidateResult(false, "q < 2");

        if (whatToValidate.n){
            if (request.getN() == null) return new ValidateResult(false, "Вы не ввели n");
        }

        if (whatToValidate.matrix) {
            if (request.getMatrix() == null) return new ValidateResult(false, "Вы не ввели матрицу");
            for (int i = 0; i < request.getMatrix().length; i++) {
                if (request.getMatrix()[i] == null) return new ValidateResult(false, "строка матрицы " + i + " пуста");
            }
        }

        if (whatToValidate.u) {
            if (request.getU() == null) return new ValidateResult(false, "Вы не ввели вектор u");
        }

        if (whatToValidate.g) {
            if (request.getG() == null) return new ValidateResult(false, "Вы не ввели вектор g");
        }

        if (whatToValidate.d) {
            if (request.getD() == null) return new ValidateResult(false, "Вы не ввели минимальное расстояние d");
        }

        if (whatToValidate.k) {
            if (request.getK() == null) return new ValidateResult(false, "Вы не ввели количество кодов (мощность множества |C|)");
        }

        return new ValidateResult(true, "");
    }

}
