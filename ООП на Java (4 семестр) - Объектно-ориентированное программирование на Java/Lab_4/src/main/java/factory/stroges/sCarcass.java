package factory.stroges;

import factory.objects.Carcass;

import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class sCarcass {
    private int size;
    private Queue<Carcass> queueCarcass;
    public sCarcass(int size){
        this.size = size;
        queueCarcass = new LinkedBlockingQueue<>(size);
        //System.out.println("sCarcass initializaded!");
    }
    public synchronized void put(Carcass entity) {
        while (queueCarcass.size() >= size)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        queueCarcass.add(entity);
        this.notifyAll();
    }
    public synchronized Carcass get() {
        while (queueCarcass.size() < 1)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        Carcass entity = queueCarcass.remove();
        this.notify();
        return entity;
    }
    public synchronized int getLimit(){
        return size;
    }
    public synchronized int getSize(){
        return queueCarcass.size();
    }
}

