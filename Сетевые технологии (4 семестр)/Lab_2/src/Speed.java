public class Speed {
    private final long PRINT_TIMEOUT = 3000;
    private final int CLIENT_ID;
    private long start;
    private long checkpointTime;
    private long byteRead;
    private long checkpointByte;
    private int currentSpeed;
    private int totalSpeed;

    public Speed(int clientId) {
        this.CLIENT_ID = clientId;
    }

    public void start() {
        this.checkpointTime = 0;
        this.checkpointByte = 0;
        this.byteRead = 0;
        start = System.currentTimeMillis();
        checkpointTime = start;
    }

    private void calculateSpeed(long currentTime) {
        currentSpeed = (int) ((byteRead - checkpointByte) / (currentTime - checkpointTime + 1));
        totalSpeed = (int) (byteRead / (currentTime - start + 1));
        checkpointTime = currentTime;
        checkpointByte = byteRead;
    }

    public void printSpeed() {
        System.out.println("Client " + CLIENT_ID + ":\n" + "Current Speed:\t" + currentSpeed + " KB/Sec\n" + "Total Speed:\t" + totalSpeed + " KB/Sec\n");
    }

    void checkSpeed(int countReadBytes) {
        byteRead += countReadBytes;
        long currentTime = System.currentTimeMillis();
        if (currentTime - checkpointTime >= PRINT_TIMEOUT) {
            calculateSpeed(currentTime);
            printSpeed();
        }
    }

    public void finish() {
        calculateSpeed(System.currentTimeMillis());
        printSpeed();
    }
}