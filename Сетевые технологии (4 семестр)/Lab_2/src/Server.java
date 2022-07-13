import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    private static int idCount = 0;
    private int port;
    private ServerSocket socket;

    public Server(String serverPort) throws IOException {
        port = Integer.parseInt(serverPort);
        socket = new ServerSocket(port, 100);
        System.out.println("Server started!");
    }

    public void receiveConnection() throws IOException {
        Socket newClientSocket = socket.accept();
        Thread translation = new Thread(new Translation(newClientSocket, idCount++));
        translation.start();
        System.out.println("Client " + idCount + " is connected.\n" + "Ip:\t" + newClientSocket.getLocalAddress().getHostAddress() + "\nPort:\t" + newClientSocket.getPort());
    }

    public static void main(String[] args) {
        try {
            Server server = new Server(args[0]);
            //System.out.println(System.currentTimeMillis());
            while(Thread.currentThread().isInterrupted() == false) {
                server.receiveConnection();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
