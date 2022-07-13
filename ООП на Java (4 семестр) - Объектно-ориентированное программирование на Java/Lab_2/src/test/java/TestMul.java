import commands.Commands;
import commands.ExeptionCommand;
import commands.Mul;
import data.Data;

import java.util.EmptyStackException;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestMul {
    @Test
    public void subTestGood() throws ExeptionCommand {
        Mul mul = new Mul();
        double a, b, c;
        Commands commands = new Commands();
        Data data = new Data();
        //15*5=75
        a = 5;
        b = 15;
        c = b * a;
        data.push(a);
        data.push(b);
        mul.execute(null, data);
        assertEquals(data.peek(), c);
        //-15*5=75
        a = 5;
        b = -15;
        c = b * a;
        data.push(a);
        data.push(b);
        mul.execute(null, data);
        assertEquals(data.peek(), c);
        //-15*0=75
        a = 0;
        b = -15;
        c = b * a;
        data.push(a);
        data.push(b);
        mul.execute(null, data);
        assertEquals(data.peek(), c);
        //3.16*7.38=23.3208
        a = 7.38;
        b = 3.16;
        c =  b * a;
        data.push(a);
        data.push(b);
        mul.execute(null, data);
        assertEquals(data.peek(), c); //23.320800000000002
        //-4.874*(-2.84)=13.84216
        a = -2.84;
        b = -4.874;
        c = b * a;
        data.push(a);
        data.push(b);
        mul.execute(null, data);
        assertEquals(data.peek(), c); //13.842159999999998
    }

    @Test
    public void subTestNotGood(){
        Mul mul = new Mul();
        Commands commands = new Commands();
        Data data = new Data();
        //Пустой стэк
        assertThrows(EmptyStackException.class, ()->mul.execute(null, data));
        //Стек с одинм числом
        data.push(2);
        assertThrows(EmptyStackException.class, ()->mul.execute(null, data));
    }
}
