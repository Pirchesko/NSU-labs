package factory.stroges;

import factory.objects.Accessories;

import java.util.Queue;
import java.util.PriorityQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class sAccessories {
    private int size;
    private Queue<Accessories>queueAccessories;
    public sAccessories(int size){
        this.size = size;
        queueAccessories = new LinkedBlockingQueue<>(size);
        //System.out.println("sAccessories initializaded!");
    }
    public synchronized void put(Accessories entity) {
        while (queueAccessories.size() >= size)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        queueAccessories.add(entity);
        this.notifyAll();
    }
    public synchronized Accessories get() {
        while (queueAccessories.size() < 1)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        Accessories entity = queueAccessories.remove();
        this.notify();
        return entity;
    }
    public synchronized int getLimit(){
        return size;
    }
    public synchronized int getSize(){
        return queueAccessories.size();
    }
}
