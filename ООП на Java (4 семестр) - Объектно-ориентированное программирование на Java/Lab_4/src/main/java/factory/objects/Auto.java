package factory.objects;

public class Auto {
    public Accessories accessories;
    public Carcass carcass;
    public Engine engine;
    public int A_ID;
    public int worker_ID;
    public Auto(int ID, int workerID, Accessories a, Carcass c, Engine e){
        accessories = a;
        carcass = c;
        engine = e;
        A_ID = ID;
        worker_ID=workerID;
    }
}
