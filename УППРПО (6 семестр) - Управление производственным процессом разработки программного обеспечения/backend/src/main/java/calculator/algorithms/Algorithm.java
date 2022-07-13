package calculator.algorithms;

import calculator.model.Matrix;

public interface Algorithm {
    public final class Result{
        public Result(boolean status, String[] answer) {
            this.status = status;
            this.answer = new String[answer.length];
            System.arraycopy(answer, 0, this.answer, 0, answer.length);
            this.answer = answer;
        }

        public Result(boolean status, String answer) {
            this.status = status;
            this.answer = new String[1];
            this.answer[0] = answer;
        }

        public boolean isStatus() {
            return status;
        }

        public void setStatus(boolean status) {
            this.status = status;
        }

        public String[] getAnswer() {
            return answer;
        }

        public String getAnswer0() {
            return answer[0];
        }

        public void setAnswer(String[] answer) {
            this.answer = answer;
        }

        private boolean status;
        private String[] answer;
    }
    final class Input{
        public Matrix matrix;
        public int action;
        public int q;
        public int n;
        public int[] u;
        public int[] g;
        public int d;
        public int k;
    }
    public Result calculate(Input input);
    public Validator.WhatToValidate whatToValidate();
}
