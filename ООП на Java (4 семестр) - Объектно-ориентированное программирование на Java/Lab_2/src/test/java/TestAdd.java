import commands.Commands;
import commands.ExeptionCommand;
import commands.Add;
import data.Data;

import java.util.EmptyStackException;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestAdd {
    @Test
    public void subTestGood() throws ExeptionCommand {
        Add add = new Add();
        double a, b, c;
        Commands commands = new Commands();
        Data data = new Data();
        //15+5=20
        a = 5;
        b = 15;
        c = b + a;
        data.push(a);
        data.push(b);
        add.execute(null, data);
        assertEquals(data.peek(), c);
        //3.16+7.38=10.54
        a = 7.38;
        b = 3.16;
        c = b + a;
        data.push(a);
        data.push(b);
        add.execute(null, data);
        assertEquals(data.peek(), c);
        //-4.874+(-2.84)=-7.714
        a = -2.84;
        b = -4.874;
        c = b + a;
        data.push(a);
        data.push(b);
        add.execute(null, data);
        assertEquals(data.peek(), c); //-7.7139999999999995
    }

    @Test
    public void subTestNotGood(){
        Add add = new Add();
        Commands commands = new Commands();
        Data data = new Data();
        //Пустой стэк
        assertThrows(EmptyStackException.class, ()->add.execute(null, data));
        //Стек с одинм числом
        data.push(2);
        assertThrows(EmptyStackException.class, ()->add.execute(null, data));
    }
}
