package analizing;

public class ExeptionChecker extends Exception{
    String message;
    public ExeptionChecker(String str){
        message = str;
    }
    @Override
    public String getMessage() {
        return message;
    }
}
