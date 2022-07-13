import java.io.IOException;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Main {

    public static void main(String[] args) throws IOException {
        String ip = "225.0.0.1";
        int port = 48654;

        Send send = new Send(ip, port);
        Receive receive = new Receive(ip, port);
        Statistic statistic = new Statistic();

        Thread sendThread = new Thread(send);
        Thread statisticThread = new Thread(statistic);

        sendThread.start();
        statisticThread.start();

        while (!Thread.currentThread().isInterrupted()) {
            statistic.checkPacket(receive.receive());
        }
    }
}
