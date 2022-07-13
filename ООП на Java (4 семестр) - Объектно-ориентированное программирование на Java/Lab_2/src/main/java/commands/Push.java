package commands;

import data.interfaceData;

import java.util.List;

public class Push implements interfaceCommands {
    @Override
    public void execute(List<String> arguments, interfaceData data) throws ExeptionCommand {
        String number = arguments.get(0);
        try {
            Double.parseDouble(number);
            data.push(Double.parseDouble(number));
        }
        catch (NumberFormatException e) {
            if (!data.isDefined(number)) {
                throw new ExeptionCommand("For number \"" + number + "\" define is not exist!");
            } else {
                data.push(data.getDefined(number));
            }
        }
    }
}
