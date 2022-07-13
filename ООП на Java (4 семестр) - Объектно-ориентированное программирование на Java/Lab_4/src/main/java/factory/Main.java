package factory;

import factory.runners.*;
import factory.stroges.sAccessories;
import factory.stroges.sAuto;
import factory.stroges.sCarcass;
import factory.stroges.sEngine;
import factory.window.VisualChecker;
import factory.window.Window;
import org.slf4j.Logger;
import threadPool.ThreadPool;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Main {
    public static Logger logger = LoggerFactory.getLogger(Main.class);

    public static void main(String[] args) {
        Window win = new Window();
        win.setVisible(true);
        PropertiesFactory settingsFactory = new PropertiesFactory();

        Storager.Checker check = new Storager.Checker();
        ThreadPool accessoriesThreadPool = new ThreadPool(settingsFactory.getCountAccessories());
        ThreadPool carcassThreadPool = new ThreadPool(settingsFactory.getCountCarcass());
        ThreadPool engineThreadPool = new ThreadPool(settingsFactory.getCountEngine());
        ThreadPool workersThreadPool = new ThreadPool(settingsFactory.getCountWorkers());
        ThreadPool dealersThreadPool = new ThreadPool(settingsFactory.getCountDealers());
        ThreadPool storagerThreadPool = new ThreadPool(settingsFactory.getCountStorager());

        sAccessories storageAccessories = new sAccessories(settingsFactory.getStorageAccessorySize());
        sCarcass storageCarcass = new sCarcass(settingsFactory.getStorageCarcassSize());
        sEngine storageEngine = new sEngine(settingsFactory.getStorageEngineSize());
        sAuto storageAuto = new sAuto(settingsFactory.getStorageAutoSize());

        VisualChecker.start(win, settingsFactory, storageAccessories, storageCarcass, storageEngine, storageAuto);

        for (int i = 0; i < settingsFactory.getCountAccessories(); i++){
            PutAccessories accessories = new PutAccessories(storageAccessories, accessoriesThreadPool.getNumberStartThread() + i);
            accessoriesThreadPool.execute(accessories);
        }
        for (int i = 0; i < settingsFactory.getCountCarcass(); i++){
            PutCarcass carcass = new PutCarcass(storageCarcass);
            carcassThreadPool.execute(carcass);
        }
        for (int i = 0; i < settingsFactory.getCountEngine(); i++){
            PutEngine engine = new PutEngine(storageEngine);
            engineThreadPool.execute(engine);
        }
        for (int i = 0; i < settingsFactory.getCountWorkers(); i++){
            Worker worker = new Worker(storageAccessories, storageCarcass, storageEngine, storageAuto, check, workersThreadPool.getNumberStartThread() + i);
            workersThreadPool.execute(worker);
        }
        for (int i = 0; i < settingsFactory.getCountDealers(); i++){
            Dealer dealer = new Dealer(storageAuto, dealersThreadPool.getNumberStartThread() + i);
            dealersThreadPool.execute(dealer);
        }
        for (int i = 0; i < settingsFactory.getCountStorager(); i++){
            Storager storager = new Storager(storageAuto, check);
            storagerThreadPool.execute(storager);
        }

        accessoriesThreadPool.runOff();
        carcassThreadPool.runOff();
        engineThreadPool.runOff();
        workersThreadPool.runOff();
        dealersThreadPool.runOff();
        storagerThreadPool.runOff();
    }
}
