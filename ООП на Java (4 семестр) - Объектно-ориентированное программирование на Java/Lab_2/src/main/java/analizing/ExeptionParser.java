package analizing;

public class ExeptionParser extends Exception{
    String message;
    public ExeptionParser(String str){
        message = str;
    }
    @Override
    public String getMessage() {
        return message;
    }
}
