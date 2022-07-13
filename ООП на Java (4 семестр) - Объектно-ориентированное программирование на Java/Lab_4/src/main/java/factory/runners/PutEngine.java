package factory.runners;

import factory.ID;
import factory.TimeSettings;
import factory.objects.Carcass;
import factory.objects.Engine;
import factory.stroges.sCarcass;
import factory.stroges.sEngine;

import static java.lang.Thread.sleep;

public class PutEngine implements Runnable{
    private sEngine storageEngine;
    public PutEngine(sEngine engine){
        storageEngine = engine;
    }
    @Override
    public void run(){
        while (true){
            try{
                sleep(TimeSettings.getEngineTime());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Engine engine = new Engine(ID.getEngineID());
            //System.out.println("PutEngine!");
            storageEngine.put(engine);
        }
    }
}
