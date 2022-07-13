package commands;

public class ExeptionCommand extends Exception{
    private String message;
    public ExeptionCommand(String str){
        message = str;
    }
    @Override
    public String getMessage() {
        return message;
    }
}
