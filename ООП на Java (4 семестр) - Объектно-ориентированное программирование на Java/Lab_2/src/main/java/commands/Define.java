package commands;

import data.interfaceData;

import java.util.List;

public class Define implements interfaceCommands{
    @Override
    public void execute(List<String> arguments, interfaceData data) throws ExeptionCommand {
        String nameNumber = arguments.get(0);
        String number = arguments.get(1);
        try {
            Double.parseDouble(number);
        } catch (NumberFormatException e) {
            throw new ExeptionCommand("This argument \"" + number + "\" is not a number");
        }
        try {
            Double.parseDouble(nameNumber);
            throw new ExeptionCommand("This argument \"" + nameNumber + "\" is not a word");
        } catch (NumberFormatException e) {
            // если мы тут, значит это слово => все в порядке
        }
        if (data.isDefined(nameNumber)) {
            System.out.println("New define " + nameNumber + " is " + number);
        }
        data.defineConstant(nameNumber, Double.parseDouble(number));
    }
}
