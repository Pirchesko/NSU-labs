import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Receive {
    private MulticastSocket socket;
    private InetAddress IP;
    private int port;
    private byte[] buf;
    public static int BUF_SIZE = 1024;

    public Receive(String IP, int port) throws IOException {
        this.socket = new MulticastSocket(port);
        this.IP = InetAddress.getByName(IP);
        this.port = port;
        this.buf = new byte[BUF_SIZE];
        socket.joinGroup(this.IP);
    }

    public DatagramPacket receive() throws IOException {
        DatagramPacket packet = new DatagramPacket(buf, BUF_SIZE);
        socket.receive(packet);
        return packet;
    }
}
