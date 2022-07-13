package commands;

import data.interfaceData;

import java.util.List;

public interface interfaceCommands {
    void execute(List<String> arguments, interfaceData data) throws ExeptionCommand;
}
