package commands;

import data.interfaceData;

import java.util.List;

public class Print implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) {
        System.out.println(data.peek());
    }
}