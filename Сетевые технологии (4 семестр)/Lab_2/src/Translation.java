import java.io.*;
import java.net.Socket;

public class Translation implements Runnable {
    private static int BUFFER_SIZE = 65536;
    private static String PATH = "D:\\Files\\IntelliJ IDEA\\Network\\Lab_2\\uploads";
    private Socket socket;
    private Speed speedCheck;
    private int clientId;

    public Translation(Socket socketServer, int id) {
        socket = socketServer;
        clientId = id;
        speedCheck = new Speed(clientId);
    }

    public void receiveData() throws IOException {
        DataInputStream dataIn = new DataInputStream(socket.getInputStream());
        DataOutputStream dataOut = new DataOutputStream(socket.getOutputStream());
        String fileName = dataIn.readUTF();
        long fileSize = dataIn.readLong();
        FileOutputStream fileOut = new FileOutputStream(PATH + File.separator + fileName);
        byte[] data = new byte[BUFFER_SIZE];
        long fileBytes = fileSize;
        speedCheck.start();
        while (fileBytes > 0) {
            int nextPart = (int) Math.min(fileBytes, BUFFER_SIZE);
            int count = dataIn.read(data, 0, nextPart);
            if (count == -1) {
                speedCheck.checkSpeed(0);
                break;
            }
            fileOut.write(data, 0, count);
            fileBytes -= count;
            speedCheck.checkSpeed(count);
        }
        speedCheck.finish();
        if (fileBytes != 0) {
            System.out.println("Something wrong in translation file!");
        } else {
            dataOut.writeUTF("File transferred successfully");
        }
        fileOut.close();
    }

    @Override
    public void run() {
        try {
            receiveData();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
