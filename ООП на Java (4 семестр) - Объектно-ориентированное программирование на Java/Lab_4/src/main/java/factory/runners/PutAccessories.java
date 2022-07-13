package factory.runners;

import factory.ID;
import factory.TimeSettings;
import factory.objects.Accessories;
import factory.stroges.sAccessories;

import static java.lang.Thread.sleep;

public class PutAccessories implements Runnable{
    private sAccessories storageAccessories;
    private int accessoriesID;
    public PutAccessories(sAccessories accessories, int ID){
        storageAccessories = accessories;
        accessoriesID = ID;
    }
    @Override
    public void run(){
        while (true){
            try{
                sleep(TimeSettings.getAccessoriesTime());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Accessories accessories = new Accessories(ID.getAccessoriesID(accessoriesID));
            //System.out.println("PutAccessories! " + (int)Thread.currentThread().getId());
            storageAccessories.put(accessories);
        }
    }
}
