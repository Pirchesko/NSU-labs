import commands.Commands;
import commands.ExeptionCommand;
import commands.Div;
import data.Data;

import java.util.EmptyStackException;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestDiv {
    @Test
    public void subTestGood() throws ExeptionCommand {
        Div div = new Div();
        double a, b, c;
        Commands commands = new Commands();
        Data data = new Data();
        //15/5=3
        a = 5;
        b = 15;
        c = b / a;
        data.push(a);
        data.push(b);
        div.execute(null, data);
        assertEquals(data.peek(), c);
        //-15/5=-3
        a = 5;
        b = -15;
        c = b / a;
        data.push(a);
        data.push(b);
        div.execute(null, data);
        assertEquals(data.peek(), c);
        //0/5=0
        a = 5;
        b = 0;
        c = b / a;
        data.push(a);
        data.push(b);
        div.execute(null, data);
        assertEquals(data.peek(), c);
        //3.16/7.38=0.42818428184
        a = 7.38;
        b = 3.16;
        c = b / a;
        data.push(a);
        data.push(b);
        div.execute(null, data);
        assertEquals(data.peek(), c);
        //-4.874/(-2.84)=13.84216
        a = -2.84;
        b = -4.874;
        c = b / a;
        data.push(a);
        data.push(b);
        div.execute(null, data);
        assertEquals(data.peek(), c);
    }

    @Test
    public void subTestNotGood(){
        Div div = new Div();
        Commands commands = new Commands();
        Data data = new Data();
        //Пустой стэк
        assertThrows(EmptyStackException.class, ()->div.execute(null, data));
        //Стек с одинм числом
        data.push(2);
        assertThrows(EmptyStackException.class, ()->div.execute(null, data));
        data.push(0);
        data.push(1);
        assertThrows(commands.ExeptionCommand.class, ()->div.execute(null, data));
    }
}
