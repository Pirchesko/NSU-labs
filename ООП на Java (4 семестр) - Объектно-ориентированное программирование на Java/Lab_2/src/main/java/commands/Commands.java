package commands;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Properties;

public class Commands {
    private static Commands commands;
    private HashMap<String, String> commandNameClass = new HashMap<>();
    public static Commands getInstance() {
        if (commands == null){
            commands = new Commands();
        }
        return commands;
    }
    public Commands() {
        Properties propNameFiles = new Properties();
        try {
            propNameFiles.load(new FileInputStream(System.getProperty("user.dir") + "\\commands\\commands.txt"));
            //properties.load(new FileInputStream(System.getProperty("user.dir") + "\\commands.txt"));
        } catch (IOException e) {
            System.err.println("Error in loading settings file in: " + System.getProperty("user.dir") + "\\commands\\commands.txt \n" + e.getMessage());
        }
        for (String commandName : propNameFiles.stringPropertyNames()) {
            String nameFile = propNameFiles.getProperty(commandName);
            Properties propNameClass = new Properties();
            try {
                propNameClass.load(new FileInputStream(System.getProperty("user.dir") + "\\commands\\" + nameFile));
                String nameClass = propNameClass.getProperty("nameClass");
                commandNameClass.put(commandName, nameClass);
            } catch (IOException e) {
                System.err.println("Error in loading settings file in: " + System.getProperty("user.dir") + "\\commands\\" + nameFile + "\n" + e.getMessage());
            }

        }

    }

    public interfaceCommands create(String command) {
        try {
            return (interfaceCommands) Class.forName(commandNameClass.get(command)).getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}