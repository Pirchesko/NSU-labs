package game.model.announcement;

import java.io.IOException;
import java.net.*;
import java.util.Arrays;
import java.util.Map;

import game.model.GameModel;
import game.model.SnakesProto.GameMessage.*;
import game.model.SnakesProto.GameMessage;
import game.model.events.EventManager;

public class AnnouncementReceiver implements Runnable {
    final String GROUP_ADDRESS = "224.0.0.7";
    final int GROUP_PORT = 8080;

    private final long RECEIVE_TIME = 3000L;
    private final long LIVE_TIME = 15000L;

    private GameModel gameModel;
    private Map<IpAddress, AnnouncementMsg> availableGames;
    private EventManager events;

    private MulticastSocket multicastSocket;

    public AnnouncementReceiver() throws IOException {
        gameModel = GameModel.getInstance();
        availableGames = gameModel.getAvailableGames();
        events = GameModel.getInstance().getEventManager();

        multicastSocket = new MulticastSocket(GROUP_PORT);
        multicastSocket.joinGroup(InetAddress.getByName(GROUP_ADDRESS));
    }

    @Override
    public void run() {
        while(!Thread.currentThread().isInterrupted()) {
            DatagramPacket receivedPacket = new DatagramPacket(new byte[64000], 6400);

            long currentTime = System.currentTimeMillis();
            long timeInterval;
            while ((timeInterval = System.currentTimeMillis() - currentTime) <= RECEIVE_TIME &&
                    (int) (RECEIVE_TIME - timeInterval) != 0) {

                try {
                    multicastSocket.setSoTimeout((int) (RECEIVE_TIME - timeInterval));
                    multicastSocket.receive(receivedPacket);

                    GameMessage gameMessage = GameMessage.parseFrom(Arrays.copyOf(receivedPacket.getData(), receivedPacket.getLength()));

                    System.out.println(gameMessage);

                    addNewGame(new IpAddress(receivedPacket.getAddress().getHostAddress()), gameMessage.getAnnouncement());
                } catch (SocketTimeoutException ignored) {

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            checkHosts();
        }
    }

    private void addNewGame(IpAddress ipAddress, AnnouncementMsg gameAnnouncementMessage) {
        for (Map.Entry<IpAddress, AnnouncementMsg> game: availableGames.entrySet()) {
            if (game.getKey().getIp().equals(ipAddress.getIp())) {
                game.getKey().updateTime();
                return;
            }
        }
            availableGames.put(ipAddress, gameAnnouncementMessage);
            events.notify("availableGames", availableGames);
        }


    private void checkHosts() {
        long currentTime = System.currentTimeMillis();

        for (Map.Entry<IpAddress, AnnouncementMsg> game: availableGames.entrySet()) {
            if ((currentTime - game.getKey().getTime()) > LIVE_TIME) {
                availableGames.remove(game.getKey());
                events.notify("list", availableGames);
                System.out.println("We will miss you, " + game.getKey().getIp() + " !");
            }
        }
    }
}
