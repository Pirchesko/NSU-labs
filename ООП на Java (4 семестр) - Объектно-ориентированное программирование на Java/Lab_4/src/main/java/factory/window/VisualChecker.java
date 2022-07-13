package factory.window;

import factory.PropertiesFactory;
import factory.stroges.sAccessories;
import factory.stroges.sAuto;
import factory.stroges.sCarcass;
import factory.stroges.sEngine;

public class VisualChecker {
    public static void start(Window win, PropertiesFactory settingsFactory, sAccessories storageAccessories, sCarcass storageCarcass, sEngine storageEngine, sAuto storageAuto){
        Thread checker = new Thread(() -> {
            while (true){
                try{
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                win.setAccessoriesInfo(storageAccessories.getSize(), storageAccessories.getLimit());
                win.setCarcassInfo(storageCarcass.getSize(), storageCarcass.getLimit());
                win.setEngineInfo(storageEngine.getSize(), storageEngine.getLimit());
                win.setAutoInfo(storageAuto.getSize(), storageAuto.getLimit());
                win.setWorkerInfo();
                win.setDealerInfo();
                win.setSettingsFactory(settingsFactory);
            }
        });
        checker.start();
    }
}
