package data;

import java.util.EmptyStackException;

public interface interfaceData {
    int     getSize();
    void    push(double number);
    double  pop() throws EmptyStackException;
    double  peek() throws EmptyStackException;
    boolean isDefined(String word);
    void    defineConstant(String word, double number);
    double  getDefined(String word);
}

