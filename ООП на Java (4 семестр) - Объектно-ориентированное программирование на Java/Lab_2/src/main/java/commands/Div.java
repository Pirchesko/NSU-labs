package commands;

import data.interfaceData;

import java.util.List;

public class Div implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) throws ExeptionCommand {
        double temp = data.pop();
        if (data.peek() == 0){
            data.push(temp);
            throw new ExeptionCommand("You are make zero division! Command was ignored! ");
        }
        data.push(temp);
        data.push(data.pop() / data.pop());
    }
}
