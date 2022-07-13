package factory.runners;

import factory.ID;
import factory.Main;
import factory.TimeSettings;
import factory.objects.Accessories;
import factory.objects.Auto;
import factory.objects.Carcass;
import factory.objects.Engine;
import factory.stroges.sAccessories;
import factory.stroges.sAuto;
import factory.stroges.sCarcass;
import factory.stroges.sEngine;

import static java.lang.Thread.sleep;

public class Worker implements Runnable{
    private sAccessories storageAccessories;
    private sCarcass storageCarcass;
    private sEngine storageEngine;
    private sAuto storageAuto;
    private Storager.Checker check;
    private int workerID;
    public Worker(sAccessories a, sCarcass c, sEngine e, sAuto auto, Storager.Checker chek, int ID){
        storageAccessories = a;
        storageCarcass = c;
        storageEngine = e;
        storageAuto = auto;
        check = chek;
        workerID = ID;
        //System.out.println("Worker initializaded!");
    }
    @Override
    public void run() {
        while (true){
            synchronized (check){
                try{
                    check.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Accessories accessories = storageAccessories.get();
            Carcass carcass = storageCarcass.get();
            Engine engine = storageEngine.get();
            try{
                sleep(TimeSettings.getWorkerTime());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Auto auto = new Auto(ID.getAutoID(), workerID, accessories, carcass, engine);
            //System.out.println("Worker!" + (int)Thread.currentThread().getId());
            storageAuto.put(auto);
        }
    }
}
