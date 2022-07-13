import commands.Commands;
import commands.ExeptionCommand;
import commands.Sqrt;
import data.Data;

import java.util.EmptyStackException;

import org.junit.Test;

import static java.lang.Math.sqrt;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestSqrt {
    @Test
    public void subTestGood() throws ExeptionCommand {
        Sqrt sqrt = new Sqrt();
        double a, b;
        Commands commands = new Commands();
        Data data = new Data();
        //sqrt(4)=2
        a = 4;
        b = sqrt(a);
        data.push(a);
        sqrt.execute(null, data);
        assertEquals(data.peek(), b);
        //sqrt(2.56)=1.6
        a = 2.56;
        b = sqrt(a);
        data.push(a);
        sqrt.execute(null, data);
        assertEquals(data.peek(), b);
        //sqrt(0)
        a = 0;
        b = sqrt(a);
        data.push(a);
        sqrt.execute(null, data);
        assertEquals(data.peek(), b);
    }

    @Test
    public void subTestNotGood(){
        Sqrt sqrt = new Sqrt();
        Commands commands = new Commands();
        Data data = new Data();
        //Пустой стэк
        assertThrows(EmptyStackException.class, ()->sqrt.execute(null, data));
        //Стек с одинм числом
        data.push(-1);
        assertThrows(commands.ExeptionCommand.class, ()->sqrt.execute(null, data));
    }
}
