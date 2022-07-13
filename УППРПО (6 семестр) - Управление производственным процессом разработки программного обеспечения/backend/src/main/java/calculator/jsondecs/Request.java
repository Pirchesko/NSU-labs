package calculator.jsondecs;

public class Request {
    private Integer[][] matrix;
    private int action;
    private Integer q;
    private Integer n;
    private Integer[] u;
    private Integer[] g;
    private Integer d;
    private Integer k;

    public Integer getN() {
        return n;
    }

    public void setN(Integer n) {
        this.n = n;
    }


    public Integer[] getU() {
        return u;
    }

    public void setU(Integer[] u) {
        this.u = u;
    }

    public Integer[] getG() {
        return g;
    }

    public void setG(Integer[] g) {
        this.g = g;
    }

    public void setQ(Integer q) {
        this.q = q;
    }

    public Integer getQ() {
        return q;
    }

    public void setMatrix(Integer[][] matrix) {
        this.matrix = matrix;
    }

    public int getAction() {
        return action;
    }

    public void setAction(int action) {
        this.action = action;
    }

    public Integer[][] getMatrix() {
        return matrix;
    }

    public Integer getD() {
        return d;
    }

    public Integer getK() {
        return k;
    }

    public void setD(Integer d) {
        this.d = d;
    }

    public void setK(Integer k) {
        this.k = k;
    }
}
