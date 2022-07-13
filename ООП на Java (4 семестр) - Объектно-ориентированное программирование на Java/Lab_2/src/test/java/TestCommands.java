import commands.Commands;
import commands.interfaceCommands;
import org.junit.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TestCommands {
    @Test
    public void create(){
        Commands commands = new Commands();
        interfaceCommands command;
        command = commands.create("PUSH");
        assertEquals("class commands.Push", command.getClass().toString());
        command = commands.create("POP");
        assertEquals("class commands.Pop", command.getClass().toString());
        command = commands.create("PRINT");
        assertEquals("class commands.Print", command.getClass().toString());
        command = commands.create("DEFINE");
        assertEquals("class commands.Define", command.getClass().toString());
        command = commands.create("+");
        assertEquals("class commands.Add", command.getClass().toString());
        command = commands.create("-");
        assertEquals("class commands.Sub", command.getClass().toString());
        command = commands.create("*");
        assertEquals("class commands.Mul", command.getClass().toString());
        command = commands.create("/");
        assertEquals("class commands.Div", command.getClass().toString());
    }
}