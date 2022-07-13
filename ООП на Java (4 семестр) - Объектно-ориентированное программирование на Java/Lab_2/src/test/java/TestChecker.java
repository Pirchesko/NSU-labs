import analizing.Arguments;
import analizing.Checker;
import analizing.ExeptionChecker;
import data.Data;
import data.interfaceData;
import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestChecker {
    @Test
    public void pars(){
        Arguments args;
        interfaceData data = new Data();
        Checker check = new Checker();
        //push
        try {
            args = check.pars("push 5", data);
            assertEquals("PUSH", args.commandName);
            assertEquals(args.arguments.size(), 1);
            assertEquals ("5", args.arguments.get(0));
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //push c
        try {
            args = check.pars("push c", data);
            assertEquals("PUSH", args.commandName);
            assertEquals(args.arguments.size(), 1);
            assertEquals("c", args.arguments.get(0));
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //pop
        try {
            args = check.pars("pop", data);
            assertEquals("POP", args.commandName);
            assertEquals(args.arguments.size(), 0);
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //print
        try {
            args = check.pars("print", data);
            assertEquals("PRINT", args.commandName);
            assertEquals(args.arguments.size(), 1);
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //define c 4
        try {
            args = check.pars("define c 4", data);
            assertEquals("DEFINE", args.commandName);
            assertEquals(args.arguments.size(), 2);
            assertEquals ("c", args.arguments.get(0));
            assertEquals("4", args.arguments.get(1));
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //sqrt 4
        try {
            args = check.pars("sqrt 4", data);
            assertEquals("SQRT", args.commandName);
            assertEquals(args.arguments.size(), 1);
            assertEquals ("4", args.arguments.get(0));
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //#SGFDS
        try {
            args = check.pars("#SGFDS", data);
            assertEquals("COMMENT", args.commandName);
            assertEquals(args.arguments.size(), 0);
        } catch (ExeptionChecker exeptionParser) {
            System.out.println(exeptionParser.getMessage());
        }
        //# SGFDS
        try {
            args = check.pars("# SGFDS", data);
            assertEquals ("COMMENT", args.commandName);
            assertEquals (args.arguments.size(), 0);
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
        //
        try {
            args = check.pars("", data);
            assertEquals("COMMENT", args.commandName);
            assertEquals(args.arguments.size(), 0);
        } catch (ExeptionChecker exeptionChecker) {
            System.out.println(exeptionChecker.getMessage());
        }
    }
    @Test
    public void badParse(){
        Arguments args;
        interfaceData data = new Data();
        Checker check = new Checker();
        assertThrows(ExeptionChecker.class, () -> check.pars("pussh a 5", data));
        assertThrows(ExeptionChecker.class, () -> check.pars("PuShA5", data));
        assertThrows(ExeptionChecker.class, () -> check.pars("jfjsddsl;g", data));
        assertThrows(ExeptionChecker.class, () -> check.pars("uuh#sdfksfd", data));
        assertThrows(ExeptionChecker.class, () -> check.pars("peek", data));
    }
}
