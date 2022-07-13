package game.model.announcement;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.List;
import java.util.TimerTask;

import game.model.GameModel;
import game.model.SnakesProto.GameMessage.*;
import game.model.SnakesProto.GamePlayer;
import game.model.SnakesProto.GameMessage;

public class AnnouncementSender extends TimerTask {
    final String GROUP_ADDRESS = "224.0.0.7";
    final int GROUP_PORT = 8080;

    private GameModel gameModel;
    private List<GamePlayer> players;

    public AnnouncementSender() throws IOException {
        gameModel = GameModel.getInstance();
        players = gameModel.getPlayers();
    }

    @Override
    public void run() {
        try {
            GameMessage gameMessage = createAnnouncementMessage();
            byte[] announcementMessageArray = gameMessage.toByteArray();
            gameModel.getDatagramSocket().send(new DatagramPacket(announcementMessageArray, announcementMessageArray.length,
                    InetAddress.getByName(GROUP_ADDRESS), GROUP_PORT));

//            System.out.println("[SERVER_MESSAGE] Server send announcementMsg with seq [" + gameMessage +
//                    "]");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private GameMessage createAnnouncementMessage() {
        GameMessage.Builder gameMessage = GameMessage.newBuilder();

        AnnouncementMsg.Builder gameAnnouncementMessage = AnnouncementMsg.newBuilder();

        gameAnnouncementMessage.setPlayers(gameModel.createGamePlayers());
        gameAnnouncementMessage.setConfig(gameModel.createGameConfig());

        gameMessage.setAnnouncement(gameAnnouncementMessage);

        gameMessage.setMsgSeq(gameModel.getServer().getServerMsgCount());
        gameModel.getServer().increaseServerMsgCount();

        return gameMessage.build();
    }
}
