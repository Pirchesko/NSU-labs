package commands;

import data.interfaceData;

import java.util.List;

public class Pop implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) {
        data.pop();
    }
}
