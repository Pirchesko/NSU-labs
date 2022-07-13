package factory.runners;

import factory.stroges.sAuto;

public class Storager implements Runnable{
    private sAuto storageAuto;
    private Checker check;
    static public class Checker{
    };
    public Storager(sAuto storageAuto, Checker check){
        this.storageAuto = storageAuto;
        this.check = check;
        //System.out.println("Storager initializaded!");
    }
    @Override
    public void run() {
        while (true) {
            synchronized (storageAuto){
                while(storageAuto.isFull() == true){
                    try {
                        storageAuto.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                synchronized (check){
                    check.notify();
                }
            }
        }
    }
}
