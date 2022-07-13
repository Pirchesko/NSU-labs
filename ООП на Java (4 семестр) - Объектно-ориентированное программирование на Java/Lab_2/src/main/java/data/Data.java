package data;

import java.util.*;

public class Data implements interfaceData{
    private Stack<Double> stack;
    private HashMap<String, Double> defineList;
    public Data(){
        this.stack = new Stack<>();
        this.defineList = new HashMap<String, Double>();
    }

    @Override
    public int getSize() {
        return stack.size();
    }

    @Override
    public final void push(double number){
        stack.push(number);
    }

    @Override
    public final double pop() throws EmptyStackException {
        return stack.pop();
    }

    @Override
    public final double peek() throws EmptyStackException {
        return stack.peek();
    }

    @Override
    public boolean isDefined(String word) {
        return defineList.containsKey(word);
    }

    @Override
    public final void defineConstant(String word, double number) {
        defineList.put(word, number);
    }

    @Override
    public final double getDefined(String word){
        return defineList.get(word);
    }
}


