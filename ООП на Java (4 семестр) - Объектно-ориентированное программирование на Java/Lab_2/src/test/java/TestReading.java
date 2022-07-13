import org.junit.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TestReading {
    @Test
    public void nextLine(){
        String[] args = {"text.txt"};
        System.out.println(args[0]);
        Reading read = new Reading(args);
        assertEquals("push 4", read.nextLine());
        assertEquals ("push 2", read.nextLine());
        assertEquals ("*", read.nextLine());
        assertEquals ("print", read.nextLine());
        assertEquals ("push 2", read.nextLine());
        assertEquals ("/", read.nextLine());
        assertEquals ("print", read.nextLine());
        assertEquals ("sqrt", read.nextLine());
        assertEquals ("print", read.nextLine());
    }
}
