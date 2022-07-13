package analizing;

import data.interfaceData;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Properties;

public class Checker {
    public ArrayList<String> arguments;
    public static Arguments pars(String str, interfaceData data) throws ExeptionChecker {
        Arguments args = new Arguments();
        if ((str.indexOf('#', 0) == 0) || (str.isEmpty() == true)){
            args.commandName = "COMMENT";
            return args;
        }
        String[] strings = str.split(" ");
        for (String s : strings) {
            args.arguments.add(s);
        }
        args.commandName = args.arguments.remove(0).toUpperCase();//может не воркать
        //
        String path = System.getProperty("user.dir") + "\\commands\\";
        Properties properties = new Properties();
        //загрузили файлик в котором есть все ссылки на другие файоы
        try {
            properties.load(new FileInputStream(path + "commands.txt"));
        } catch (IOException e) {
            System.err.println("Error in open settings file! " + e.getMessage());
        }
        //загружаем файл нужной нам команды
        try {
            properties.load(new FileInputStream(path + properties.getProperty(args.commandName)));
        } catch (IOException e) {
            throw new ExeptionChecker("Command \"" + args.commandName + "\" is not exist!");
        }
        //чекаем количество вргументов
        int countArgs = Integer.parseInt(properties.getProperty("countArgs"));
        if (countArgs > args.arguments.size()){
            throw new ExeptionChecker("You are not write arguments or they are not enough!");
        }
        else if (countArgs < args.arguments.size()){
            System.out.println("You are write too much arguments, the latter arguments will be ignored!");
            for (int i = countArgs; i < args.arguments.size(); i++){
                args.arguments.remove(i);
            }
        }
        //чекаем количество чиселок на стэке
        int needArgs = Integer.parseInt(properties.getProperty("needArgs"));
        if (needArgs > data.getSize()){
            throw new ExeptionChecker("Your are have not many arguments for this operation: " + args.commandName);
        }
        return args;
    }
}
