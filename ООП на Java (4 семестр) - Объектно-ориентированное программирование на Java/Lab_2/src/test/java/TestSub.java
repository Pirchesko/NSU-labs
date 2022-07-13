import commands.Commands;
import commands.ExeptionCommand;
import commands.Sub;
import data.Data;

import java.util.EmptyStackException;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestSub {
    @Test
    public void subTestGood() throws ExeptionCommand {
        Sub sub = new Sub();
        double a, b, c;
        Commands commadns = new Commands();
        Data data = new Data();
        //15-5=10
        a = 5;
        b = 15;
        c = b - a;
        data.push(a);
        data.push(b);
        sub.execute(null, data);
        assertEquals(data.peek(), c);
        //3.16-7.38=-4.22
        a = 7.38;
        b = 3.16;
        c = b - a;
        data.push(a);
        data.push(b);
        sub.execute(null, data);
        assertEquals(data.peek(), c);
        //-4.874-(-2.84)=-2.034
        a = -2.84;
        b = -4.874;
        c = b - a;
        data.push(a);
        data.push(b);
        sub.execute(null, data);
        assertEquals(data.peek(), c);
    }

    @Test
    public void subTestNotGood(){
        Sub sub = new Sub();
        Commands commadns = new Commands();
        Data data = new Data();
        //Пустой стэк
        assertThrows(EmptyStackException.class, ()->sub.execute(null, data));
        //Стек с одинм числом
        data.push(2);
        assertThrows(EmptyStackException.class, ()->sub.execute(null, data));
    }
}
