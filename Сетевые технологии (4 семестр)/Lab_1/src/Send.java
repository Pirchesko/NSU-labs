import java.io.IOException;
import java.net.*;

public class Send implements Runnable {
    private DatagramSocket socket = new DatagramSocket();
    private InetAddress IP;
    private int port;
    private static int PERIOD = 500;

    Send(String IP, int port) throws UnknownHostException, SocketException {
        this.IP = InetAddress.getByName(IP);
        this.port = port;
    }

    public void run() {
        byte[] buf = "Some data".getBytes();
        DatagramPacket packet = new DatagramPacket(buf, buf.length, this.IP, this.port);
        try {
            while (!Thread.currentThread().isInterrupted()) {
                this.socket.send(packet);
                Thread.sleep(PERIOD);
            }
        } catch (InterruptedException | IOException e) {
            e.printStackTrace();
        }
    }
}
