package factory.stroges;

import factory.objects.Engine;

import java.util.Queue;
import java.util.PriorityQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class sEngine {
    private int size;
    private Queue<Engine> queueEngine;
    public sEngine(int size){
        this.size = size;
        queueEngine = new LinkedBlockingQueue<>(size);
        //System.out.println("sEngine initializaded!");
    }
    public synchronized void put(Engine entity) {
        while (queueEngine.size() >= size)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        queueEngine.add(entity);
        this.notifyAll();
    }
    public synchronized Engine get() {
        while (queueEngine.size() < 1)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        Engine entity = queueEngine.remove();
        this.notify();
        return entity;
    }
    public synchronized int getLimit(){
        return size;
    }
    public synchronized int getSize(){
        return queueEngine.size();
    }
}
