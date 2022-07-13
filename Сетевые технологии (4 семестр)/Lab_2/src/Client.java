import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class Client {
    private static int BUFFER_SIZE = 65536;
    private InetAddress ip;
    private int port;
    private Socket socket;

    public Client(String ipServer, String portServer) throws IOException {
        ip = InetAddress.getByName(ipServer);
        port = Integer.parseInt(portServer);
        socket = new Socket(ip, port);
    }

    public void sendData(String path) throws IOException {
        File file = new File(path);
        FileInputStream fileIn = new FileInputStream(file);
        DataOutputStream dataOut = new DataOutputStream(socket.getOutputStream());
        DataInputStream dataIn = new DataInputStream(socket.getInputStream());
        dataOut.writeUTF(file.getName());
        dataOut.writeLong(file.length());
        byte[] data = new byte[BUFFER_SIZE];
        int size;
        while ((size = fileIn.read(data)) != -1) {
            dataOut.write(data, 0, size);
        }
        dataOut.flush();
        String confirmation = dataIn.readUTF();
        System.out.println(confirmation); //???
    }

    public static void main(String[] args) {
        try {
            Client client = new Client(args[0], args[1]);
            client.sendData(args[2]);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
