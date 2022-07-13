package factory.runners;

import factory.Main;
import factory.TimeSettings;
import factory.objects.Auto;
import factory.stroges.sAuto;

import java.sql.Time;

import static java.lang.Thread.sleep;

public class Dealer implements Runnable{
    private sAuto storageAuto;
    private int dealerID;
    public Dealer(sAuto auto, int ID){
        storageAuto = auto;
        dealerID = ID;
        //System.out.println("Dealer initializaded!");
    }
    @Override
    public void run(){
        while(true){
            try{
                sleep(TimeSettings.getDealerTime());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (storageAuto){
                Auto auto = storageAuto.get();
                Main.logger.info("Dealer {}: Auto {} (Accessory: {} Carcass: {} Engine: {}) by Worker {}", dealerID, auto.A_ID, auto.accessories.aID, auto.carcass.cID, auto.engine.eID, auto.worker_ID);
                //System.out.println("Dealer!" + Thread.currentThread().getId());
                storageAuto.notify();
            }
        }
    }
}
