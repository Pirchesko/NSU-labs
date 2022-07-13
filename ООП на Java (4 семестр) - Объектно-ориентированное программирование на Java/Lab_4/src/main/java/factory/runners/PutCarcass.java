package factory.runners;

import factory.ID;
import factory.TimeSettings;
import factory.objects.Accessories;
import factory.objects.Carcass;
import factory.stroges.sAccessories;
import factory.stroges.sCarcass;

import static java.lang.Thread.sleep;

public class PutCarcass implements Runnable{
    private sCarcass storageCarcass;
    public PutCarcass(sCarcass carcass){
        storageCarcass = carcass;
    }
    @Override
    public void run(){
        while (true){
            try{
                sleep(TimeSettings.getCarcassTime());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Carcass carcass = new Carcass(ID.getCarcassID());
            //System.out.println("PutCarcass!");
            storageCarcass.put(carcass);
        }
    }
}
