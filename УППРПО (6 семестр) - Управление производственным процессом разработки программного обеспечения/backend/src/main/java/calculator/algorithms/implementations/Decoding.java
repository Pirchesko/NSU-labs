package calculator.algorithms.implementations;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.model.Matrix;
import calculator.model.Vector;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

@Component
public class Decoding implements Algorithm {
    @Override
    public Result calculate(Input input) {
        if (input.u.length != input.matrix.getColumns()){
            return new Result(false, "Длина вектора u должна быть равна числу колонок матрицы G");
        }

        // приняли G
        GtoH gtoH = new GtoH();
        Matrix H = gtoH.toGorH(input.matrix);
        if (gtoH.msg != ""){
            return new Result(false, gtoH.msg);
        }
        // заполним мапу всех кодовых слов
        int countVec = (int)Math.pow(input.matrix.getQ(), input.matrix.getLines());
        Vector v = new Vector(new int[input.matrix.getLines()],input.matrix.getQ());
        Arrays.fill(v.data, 0);
        HashMap<int[], int[]> inAndOut = new HashMap<>();

        for (int i = 0; i < countVec; i++) {
            Vector code = v.mulToMatrix(input.matrix);
            int[] a = new int[v.data.length];
            System.arraycopy(v.data,0,a,0,v.data.length);
            int[] b = new int[code.data.length];
            System.arraycopy(code.data,0,b,0,code.data.length);
            inAndOut.put(a,b);
            v.increase();
        }
        System.out.println("Матрица H");
        gtoH.printMatrix(H);

        // посчитаем синдром нашего вектора
        Vector syndrom = H.mulToVec(input.u);
        if (!syndrom.isZero())
            return new Result(false, "Синдром не нулевой" + syndrom);

        // найдем его в мапе
        for(Map.Entry<int[], int[]> entry : inAndOut.entrySet()) {
            int[] open = entry.getKey();
            int[] code = entry.getValue();
            if (Arrays.equals(code,input.u))
                return new Result(true, (new Vector(open,input.matrix.getQ())).toString());
        }

        return new Result(false, "Синдром нулевой, но введенный Вами вектор не принадлежит данному коду");
    }

    @Override
    public Validator.WhatToValidate whatToValidate() {
        Validator.WhatToValidate whatToValidate = new Validator.WhatToValidate();
        whatToValidate.matrix = true;
        whatToValidate.u = true;
        return whatToValidate;
    }
}
