import analizing.Arguments;
import analizing.ExeptionChecker;
import analizing.Checker;
import commands.Commands;
import commands.ExeptionCommand;
import data.Data;
import data.interfaceData;

public class Main {
    public static void main(String[] args) {
        Reading read = new Reading(args);
        String str;
        interfaceData data = new Data();
        Commands commands = new Commands();
        Checker check = new Checker();
        while((str = read.nextLine()) != ""){
            Arguments arguments;
            try {
                arguments = check.pars(str, data);
                try {
                    commands.create(arguments.commandName).execute(arguments.arguments, data);
                } catch (ExeptionCommand exeptionCommand) {
                    System.out.println(exeptionCommand.getMessage());
                }
            } catch (ExeptionChecker exeptionChecker) {
                System.out.println(exeptionChecker.getMessage());
            }
        }
    }
}
