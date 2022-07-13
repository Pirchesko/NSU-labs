package commands;

import data.interfaceData;

import java.util.List;

public class Sqrt implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) throws ExeptionCommand {
        if (data.peek() < 0) {
            throw new ExeptionCommand("You are make SQRT when number is <0! Command was ignored! ");
        }
        data.push(Math.sqrt(data.pop()));
    }
}