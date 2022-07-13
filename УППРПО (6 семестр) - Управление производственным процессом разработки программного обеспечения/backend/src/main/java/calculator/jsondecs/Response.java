package calculator.jsondecs;

public class Response {
    private boolean status;
    private String errorMessage;
    private String[] plainText;
    private String wolfram;

    public Response(boolean status, String errorMessage, String[] plainText, String wolfram) {
        this.status = status;
        this.errorMessage = errorMessage;
        if (plainText != null){
            this.plainText = new String[plainText.length];
            System.arraycopy(plainText, 0, this.plainText, 0, plainText.length);
        }
        this.wolfram = wolfram;
    }

    public String[] getPlainText() {
        return plainText;
    }

    public void setPlainText(String[] plainText) {
        this.plainText = plainText;
    }

    public String getWolfram() {
        return wolfram;
    }

    public void setWolfram(String wolfram) {
        this.wolfram = wolfram;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setStatus(boolean status) {
        this.status = status;
    }

    public boolean isStatus() {
        return status;
    }


}
