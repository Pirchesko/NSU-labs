package commands;

import data.interfaceData;

import java.util.List;

public class Add implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) throws ExeptionCommand {
        data.push(data.pop() + data.pop());
    }
}
