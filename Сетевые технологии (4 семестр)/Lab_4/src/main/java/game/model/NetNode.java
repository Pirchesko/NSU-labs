package game.model;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Arrays;


public class NetNode {
    GameModel gameModel;

    NetNode() throws IOException {
        gameModel = GameModel.getInstance();
    }

    void sendConfirmation(InetAddress inetAddress, int port, DatagramPacket receivedPacket) throws IOException {
        SnakesProto.GameMessage gameMessage = SnakesProto.GameMessage.parseFrom(Arrays.copyOf(receivedPacket.getData(), receivedPacket.getLength()));

        byte[] buf = createAckMsg(gameMessage.getMsgSeq()).toByteArray();

        gameModel.getDatagramSocket().send(new DatagramPacket(buf, buf.length, inetAddress, port));
    }

    private SnakesProto.GameMessage createAckMsg(long msgSeq) {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        gameMessage.setAck(SnakesProto.GameMessage.AckMsg.newBuilder().build());

        gameMessage.setMsgSeq(msgSeq);

        return gameMessage.build();
    }

}
