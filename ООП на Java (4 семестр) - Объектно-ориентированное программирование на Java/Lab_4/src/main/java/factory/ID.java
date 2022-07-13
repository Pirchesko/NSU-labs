package factory;

public class ID {
    private static int accessoriesID;
    private static int carcassID;
    private static int engineID;
    private static int autoID;
    public ID(){
        accessoriesID = 0;
        carcassID = 0;
        engineID = 0;
        autoID = 0;
        System.out.println("sfsfs");
    }
    public synchronized static int getAccessoriesID(int number) {
        accessoriesID++;
        return number * 10000 + accessoriesID;
    }
    public synchronized static int getCarcassID() {
        carcassID++;
        return carcassID;
    }
    public synchronized static int getEngineID() {
        engineID++;
        return engineID;
    }
    public synchronized static int getAutoID() {
        autoID++;
        return autoID;
    }
}
