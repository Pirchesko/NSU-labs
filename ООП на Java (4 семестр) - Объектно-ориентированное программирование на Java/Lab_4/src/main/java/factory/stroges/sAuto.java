package factory.stroges;

import factory.objects.Auto;

import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class sAuto {
    private int size;
    private Queue<Auto> queueAuto;
    public sAuto(int size){
        this.size = size;
        queueAuto = new LinkedBlockingQueue<>(size);
        //System.out.println("sAuto initializaded!");
    }
    public synchronized void put(Auto entity) {
        while (queueAuto.size() >= size)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        queueAuto.add(entity);
        this.notifyAll();
    }
    public synchronized Auto get() {
        while (queueAuto.size() < 1)
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        Auto entity = queueAuto.remove();
        this.notify();
        return entity;
    }
    public boolean isFull(){
        if (queueAuto.size() >= size) {
            return true;
        } else {
            return false;
        }
    }
    public synchronized int getLimit(){
        return size;
    }
    public synchronized int getSize(){
        return queueAuto.size();
    }
}
