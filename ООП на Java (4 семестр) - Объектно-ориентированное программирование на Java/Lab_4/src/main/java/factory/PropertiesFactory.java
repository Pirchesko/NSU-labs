package factory;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class PropertiesFactory {
    private String path = System.getProperty("user.dir") + "\\settings\\";
    private int countAccessories;
    private int countCarcass;
    private int countEngine;
    private int countWorkers;
    private int countDealers;
    private int countStorager;
    private int storageAccessorySize;
    private int storageCarcassSize;
    private int storageEngineSize;
    private int storageAutoSize;
    private boolean logSale;
    public PropertiesFactory(){
        Properties properties = new Properties();
        try {
            properties.load(new FileInputStream(path + "factory.txt"));
        } catch (IOException e) {
            System.err.println("Error in open settings file! " + e.getMessage());
        }
        countAccessories = Integer.parseInt(properties.getProperty("AccessorySuppliers"));
        countCarcass = 1;
        countEngine = 1;
        countWorkers = Integer.parseInt(properties.getProperty("Workers"));
        countDealers = Integer.parseInt(properties.getProperty("Dealers"));
        countStorager = 1;
        storageAccessorySize = Integer.parseInt(properties.getProperty("StorageAccessorySize"));
        storageCarcassSize = Integer.parseInt(properties.getProperty("StorageCarcassSize"));
        storageEngineSize = Integer.parseInt(properties.getProperty("StorageEngineSize"));
        storageAutoSize = Integer.parseInt(properties.getProperty("StorageAutoSize"));
        logSale = Boolean.parseBoolean(properties.getProperty("LogSale"));
    }
    public int getCountAccessories() {
        return countAccessories;
    }
    public int getCountCarcass() {
        return countCarcass;
    }
    public int getCountEngine() {
        return countEngine;
    }
    public int getCountWorkers() {
        return countWorkers;
    }
    public int getCountDealers() {
        return countDealers;
    }
    public int getCountStorager(){
        return countStorager;
    }
    public int getStorageAccessorySize() {
        return storageAccessorySize;
    }
    public int getStorageCarcassSize() {
        return storageCarcassSize;
    }
    public int getStorageEngineSize() {
        return storageEngineSize;
    }
    public int getStorageAutoSize() {
        return storageAutoSize;
    }
    public boolean getLogSale() {
        return logSale;
    }
}



