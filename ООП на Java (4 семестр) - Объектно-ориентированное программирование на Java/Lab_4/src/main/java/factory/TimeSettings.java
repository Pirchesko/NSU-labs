package factory;

public class TimeSettings {
    private static int accessoriesTime = 5000;
    private static int carcassTime = 1000;
    private static int engineTime = 1000;
    private static int workerTime = 15000;
    private static int dealerTime = 15000;

    public static void setAccessoriesTime(int accessoriesTime) {
        TimeSettings.accessoriesTime = accessoriesTime;
    }
    public static void setCarcassTime(int carcassTime) {
        TimeSettings.carcassTime = carcassTime;
    }
    public static void setEngineTime(int engineTime) {
        TimeSettings.engineTime = engineTime;
    }
    public static void setWorkerTime(int workerTime) {
        TimeSettings.workerTime = workerTime;
    }
    public static void setDealerTime(int dealerTime) {
        TimeSettings.dealerTime = dealerTime;
    }

    public static int getAccessoriesTime() {
        return accessoriesTime;
    }
    public static int getCarcassTime() {
        return carcassTime;
    }
    public static int getEngineTime() {
        return engineTime;
    }
    public static int getWorkerTime() {
        return workerTime;
    }
    public static int getDealerTime() {
        return dealerTime;
    }
}
