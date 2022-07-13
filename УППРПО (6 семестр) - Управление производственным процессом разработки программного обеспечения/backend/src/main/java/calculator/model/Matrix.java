package calculator.model;

public class Matrix {
    public int[][] data;
    private int q;
    private final int lines;
    private final int columns;

    public Matrix(int lines, int columns) {
        this.lines = lines;
        this.columns = columns;
        data = new int[lines][columns];
    }

    public Matrix(int lines, int columns, int q, int init) {
        this.lines = lines;
        this.columns = columns;
        this.q = q;
        data = new int[lines][columns];
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                this.data[i][j] = init;
            }
        }
    }

    public Matrix(int[][] data, int q) {
        this.q = q;
        lines = data.length;
        columns = data[0].length;
        this.data = new int[lines][columns];

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                this.data[i][j] = data[i][j]%q;
            }
        }
    }

    public Matrix(Integer[][] data, int q) {
        this.q = q;
        lines = data.length;
        columns = data[0].length;
        this.data = new int[lines][columns];

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                this.data[i][j] = data[i][j]%q;
            }
        }
    }

    public int getQ() {
        return q;
    }

    public int getLines() {
        return lines;
    }

    public int getColumns() {
        return columns;
    }

    public int[][] getData() {
        return data;
    }

    public Matrix mul(Matrix rightMatrix){
        if (columns != rightMatrix.lines){
            return null;
        }
        Matrix result = new Matrix(lines, rightMatrix.columns);
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < rightMatrix.columns; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < columns; k++)
                    result.data[i][j] += (this.data[i][k] * rightMatrix.data[k][j]) % q;
            }
        }
        return result;
    }

    public Vector mulToVec(int[] vector){
        Vector res = new Vector(q, this.lines);
        for (int i = 0; i < this.lines; i++) {
            for (int j = 0; j < this.columns; j++) {
                res.data[i] += this.data[i][j]*vector[j] % q;
            }
            res.data[i] %= q;
        }
        return res;
    }

    public String[] toStringArray(){
        String[] str = new String[lines];
        for (int i = 0; i < lines; i++) {
            StringBuilder stringBuilder = new StringBuilder();
            for (int j = 0; j < columns; j++) {
                stringBuilder.append(data[i][j]);
                stringBuilder.append(" ");
            }
            str[i] = stringBuilder.toString();
        }
        return str;
    }

    public Matrix toModule(Matrix matrix){
        for (int i = 0; i < matrix.getLines(); i++) {
            for (int j = 0; j < matrix.getColumns(); j++) {
                if (matrix.data[i][j] < 0) {
                    while (matrix.data[i][j] < 0) {
                        matrix.data[i][j] += matrix.getQ();
                    }
                }
                else if (matrix.data[i][j] >= matrix.getQ()) {
                    matrix.data[i][j] = matrix.data[i][j] % matrix.getQ();
                }
            }
        }
        return matrix;
    }

}
