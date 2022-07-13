package game.model;

import game.Main;
import game.model.announcement.AnnouncementReceiver;
import game.model.announcement.AnnouncementSender;
import game.model.announcement.IpAddress;
import game.model.events.EventManager;
import game.model.exceptions.WrongArgException;

import java.io.IOException;
import java.net.*;
import java.util.*;
import java.util.concurrent.Semaphore;

public class GameModel {
    private static GameModel instance;

    private final long ANNOUNCEMENT_PERIOD = 1000L;
    private int SELF_PORT;

    private SnakesProto.NodeRole nodeRole;

    private DatagramSocket datagramSocket;

    private EventManager events;
    private EventManager netEvents;

    private Map<IpAddress, SnakesProto.GameMessage.AnnouncementMsg> availableGames;

    private List<SnakesProto.GamePlayer> players;
    private List<SnakesProto.GameState.Snake> snakes;
    private List<SnakesProto.GameState.Coord> food;

    public List<SnakesProto.GamePlayer> save_players;
    public List<SnakesProto.GameState.Snake> save_snakes;
    public List<SnakesProto.GameState.Coord> save_food;

    private String playerName;

    private Field gameField;

    private int fieldHeight;
    private int fieldWidth;
    private int foodStatic;
    private float foodPerPlayer;
    private int stateDelayMs;
    private float deadFoodProb;
    private int pingDelayMs;
    private int nodeTimeoutMs;
    private int maxPlayers;

    private Timer announcementMessageSender;
    private Thread announcementMessagesReceiver;

    private Thread netReceiver;
    private Timer resendServerConfirmationTimerTask;
    private Timer resendClientServerConfirmationTimerTask;

    private Server server;
    private ClientServer clientServer;

    private Thread gameLogic;

    private List<GameMessageWrapper> unconfirmedMessagesFromClient;
    private List<GameMessageWrapper> unconfirmedMessagesFromServer;

    private Semaphore clientUnconfirmedMessagesSem;
    private Semaphore serverUnconfirmedMessagesSem;

    private long clientServerMsgCount;

    private boolean changeServer;

    private GameModel() throws IOException {
        SELF_PORT = Main.port;
        datagramSocket = new DatagramSocket(SELF_PORT);

        events = new EventManager("availableGames", "gameState", "message", "serverDown");
        netEvents = new EventManager("receivedPacket");

        availableGames = Collections.synchronizedMap(new HashMap<>());

        players = Collections.synchronizedList(new ArrayList<>());
        snakes = Collections.synchronizedList(new ArrayList<>());
        food = Collections.synchronizedList(new ArrayList<>());

        playerName = "Player";

        initGameSettings();

        unconfirmedMessagesFromClient = Collections.synchronizedList(new ArrayList<>());
        unconfirmedMessagesFromServer = Collections.synchronizedList(new ArrayList<>());

        clientUnconfirmedMessagesSem = new Semaphore(1);
        serverUnconfirmedMessagesSem = new Semaphore(1);

        clientServerMsgCount = 0;
    }

    public boolean isChangeServer() {
        return changeServer;
    }

    public void setChangeServer(boolean changeServer) {
        this.changeServer = changeServer;
    }

    private void initGameSettings() {
        fieldHeight = 10;
        fieldWidth = 10;
        foodStatic = 1;
        foodPerPlayer = 1;
        stateDelayMs = 500;
        deadFoodProb = 0.1f;
        pingDelayMs = 3000;
        nodeTimeoutMs = 800;
        maxPlayers = 8;
    }

    public static synchronized GameModel getInstance() throws IOException {
        if (instance == null) {
            instance = new GameModel();
        }
        return instance;
    }

    public int getSelfPort() {
        return SELF_PORT;
    }

    public void setNodeRole(SnakesProto.NodeRole nodeRole) {
        this.nodeRole = nodeRole;
    }

    public DatagramSocket getDatagramSocket() {
        return datagramSocket;
    }

    public EventManager getEventManager() {
        return events;
    }

    public EventManager getNetEventManager() {
        return netEvents;
    }

    public ClientServer getClientServer() {
        return clientServer;
    }

    public Server getServer() {
        return server;
    }

    public Map<IpAddress, SnakesProto.GameMessage.AnnouncementMsg> getAvailableGames() {
        return availableGames;
    }

    public List<SnakesProto.GamePlayer> getPlayers() {
        return players;
    }

    public List<SnakesProto.GameState.Snake> getSnakes() {
        return snakes;
    }

    public List<SnakesProto.GameState.Coord> getFood() {
        return food;
    }

    public Field getGameField() {
        return gameField;
    }

    private void createGameField() throws IOException {
        gameField = new Field();

        System.out.println("Game field created");
    }

    public String getMasterPlayerName() {
        return playerName;
    }

    public int getFieldHeight() {
        return fieldHeight;
    }

    public int getFieldWidth() {
        return fieldWidth;
    }

    public int getFoodStatic() {
        return foodStatic;
    }

    public float getFoodPerPlayer() {
        return foodPerPlayer;
    }

    public int getStateDelayMs() {
        return stateDelayMs;
    }

    public float getDeadFoodProb() {
        return deadFoodProb;
    }

    public int getPingDelayMs() {
        return pingDelayMs;
    }

    public int getNodeTimeoutMs() {
        return nodeTimeoutMs;
    }

    public int getMaxPlayers() {
        return maxPlayers;
    }

    public long getClientServerMsgCount() {
        return clientServerMsgCount;
    }

    public void increaseClientServerMsgCount() {
        clientServerMsgCount++;
    }

    public void setMasterPlayerName(String masterPlayerName) {
        playerName = masterPlayerName;
    }

    public void setFieldHeight(int fieldHeight) {
        if (fieldHeight > 100) {
            throw new WrongArgException("Field size must not exceed 100");
        } else if (fieldHeight < 3) {
            throw new WrongArgException("Field size must not be less than 3");
        }
        this.fieldHeight = fieldHeight;
    }

    public void setFieldWidth(int fieldWidth) {
        if (fieldWidth > 100) {
            throw new WrongArgException("Field size must not exceed 100");
        } else if (fieldWidth < 3) {
            throw new WrongArgException("Field size must not be less than 3");
        }
        this.fieldWidth = fieldWidth;
    }

    public void setFoodStatic(int foodStatic) {
        this.foodStatic = foodStatic;
    }

    public void setFoodPerPlayer(float foodPerPlayer) {
        this.foodPerPlayer = foodPerPlayer;
    }

    public void setStateDelayMs(int stateDelayMs) {
        this.stateDelayMs = stateDelayMs;
    }

    public void setDeadFoodProb(float deadFoodProb) {
        this.deadFoodProb = deadFoodProb;
    }

    public void setPingDelayMs(int pingDelayMs) {
        this.pingDelayMs = pingDelayMs;
    }

    public void setNodeTimeoutMs(int nodeTimeoutMs) {
        this.nodeTimeoutMs = nodeTimeoutMs;
    }

    public void setMaxPlayers(int maxPlayers) {
        if (maxPlayers > 32) {
            throw new WrongArgException("Max number of players must not exceed 32");
        }
        this.maxPlayers = maxPlayers;
    }

    public void startGame() throws IOException {
        createClientServer();
        startNetReceiver();
    }

    public void startServerLogic() throws IOException {
        createGameField();
        createServer();
        startGameLogic();
    }

    private void createServer() throws IOException {
        server = new Server(serverUnconfirmedMessagesSem);

//        resendServerConfirmationTimerTask = new Timer();
//        resendServerConfirmationTimerTask.schedule(new ResendServerConfirmationTimerTask(), 5000, 5000);

        System.out.println("Server confirmation resender is working");

        sendAnnouncementMessage();
    }

    private void createClientServer() throws IOException {
        clientServer = new ClientServer(pingDelayMs, clientUnconfirmedMessagesSem);

//        resendClientServerConfirmationTimerTask = new Timer();
//        resendClientServerConfirmationTimerTask.schedule(new ResendClientServerConfirmationTimerTask(), 5000, 5000);

        System.out.println("Client confirmation resender is working");
    }

    private void sendAnnouncementMessage() throws IOException {
        announcementMessageSender = new Timer();
        announcementMessageSender.schedule(new AnnouncementSender(), 0, ANNOUNCEMENT_PERIOD);
        System.out.println("Start sending announcement messages");
    }

    private void stopSendingAnnouncementMessage() {
        announcementMessageSender.cancel();
        announcementMessageSender.purge();

        announcementMessageSender = null;

        System.out.println("Stop sending announcement messages");
    }

    public void receiveAnnouncementMessages() throws IOException {
        announcementMessagesReceiver = new Thread(new AnnouncementReceiver());
        announcementMessagesReceiver.start();
        System.out.println("Start receiving announcement messages");
    }

    public void stopReceivingAnnouncementMessage() {
        announcementMessagesReceiver.interrupt();
        System.out.println("Stop receiving announcement messages");
    }

    private void startNetReceiver() {
        netReceiver = new Thread(new NetReceiver());
        netReceiver.start();

        System.out.println("Net receiver is working");
    }

    private void stopNetReceiver() {
        netReceiver.interrupt();

        netReceiver = null;

        System.out.println("Net receiver is down");

        System.out.println("Resend confirmation TimerTask is down");
    }

    private void startGameLogic() {
        gameLogic = new Thread(new GameLogic());
        gameLogic.start();

        System.out.println("Game logic is working");
    }

    private void stopGameLogic() {
        gameLogic.interrupt();
        gameLogic = null;

        System.out.println("Game logic is down");
    }

    private void endServer() {
        netEvents.unsubscribe(server, "receivedPacket");
        System.out.println("Сервер отписался от события [receivedPacket]");
        if (gameLogic != null) {
            stopGameLogic();
        }

        if (announcementMessageSender != null) {
            stopSendingAnnouncementMessage();
        }

        if (resendServerConfirmationTimerTask != null) {
            resendServerConfirmationTimerTask.cancel();
            resendServerConfirmationTimerTask.purge();

            resendServerConfirmationTimerTask = null;
        }

        server = null;

        gameField = null;
    }

    public void endGame() {
        endServer();

        if (resendClientServerConfirmationTimerTask != null) {
            resendClientServerConfirmationTimerTask.cancel();
            resendClientServerConfirmationTimerTask.purge();

            resendClientServerConfirmationTimerTask = null;
        }

        stopNetReceiver();

        players.clear();
        snakes.clear();
        food.clear();

        availableGames.clear();

        unconfirmedMessagesFromClient.clear();

        clientServerMsgCount = 0;

        initGameSettings();

        clientServer.stopPingTimerTask();
        clientServer = null;

        events.unsubscribeAll("availableGames", "gameState", "message");
        netEvents.unsubscribe(clientServer, "receivedPacket");
    }

    public class NetReceiver implements Runnable {
        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    DatagramPacket receivedPacket = new DatagramPacket(new byte[64000], 0, 64000);
                    datagramSocket.receive(receivedPacket);

                    netEvents.notify("receivedPacket", receivedPacket);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public class GameLogic implements Runnable {
        @Override
        public void run() {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    List<SnakesProto.GameState.Snake> newSnakes = new ArrayList<>();
                    List<SnakesProto.GameState.Snake> finalSnakes = new ArrayList<>();

                    List<SnakesProto.GamePlayer> newPlayers = new ArrayList<>();

                    List<Integer> snakesToDelete = new ArrayList<>();

                    boolean changeServer = false;

                    for (SnakesProto.GameState.Snake snake : snakes) {
                        for (int i = 0; i < snake.getPointsList().size(); i++) {
                            gameField.setEmptyCell(snake.getPointsList().get(i).getY(), snake.getPointsList().get(i).getX());
                        }
                    }

                    for (SnakesProto.GameState.Snake snake : snakes) {
                        List<SnakesProto.GameState.Coord> points = new ArrayList<>(snake.getPointsList());

                        if (snake.getHeadDirection() == SnakesProto.Direction.UP) {
                            if (snake.getPoints(0).getY() - 1 < 0) {
                                points.add(0, createCoord(snake.getPoints(0).getX(), fieldHeight - 1));
                            } else {
                                points.add(0, createCoord(snake.getPoints(0).getX(), snake.getPoints(0).getY() - 1));
                            }
                        } else if (snake.getHeadDirection() == SnakesProto.Direction.DOWN) {
                            if (snake.getPoints(0).getY() + 1 >= fieldHeight) {
                                points.add(0, createCoord(snake.getPoints(0).getX(), 0));
                            } else {
                                points.add(0, createCoord(snake.getPoints(0).getX(), snake.getPoints(0).getY() + 1));
                            }
                        } else if (snake.getHeadDirection() == SnakesProto.Direction.LEFT) {
                            if (snake.getPoints(0).getX() - 1 < 0) {
                                points.add(0, createCoord(fieldWidth - 1, snake.getPoints(0).getY()));
                            } else {
                                points.add(0, createCoord(snake.getPoints(0).getX() - 1, snake.getPoints(0).getY()));
                            }
                        } else if (snake.getHeadDirection() == SnakesProto.Direction.RIGHT) {
                            if (snake.getPoints(0).getX() + 1 >= fieldWidth) {
                                points.add(0, createCoord(0, snake.getPoints(0).getY()));
                            } else {
                                points.add(0, createCoord(snake.getPoints(0).getX() + 1, snake.getPoints(0).getY()));
                            }
                        }

                        //Съела ли конкретная змея саму себя
                        boolean eatYourself = false;

                        for (int i = 1; i < points.size(); i++) {
                            if (points.get(0).equals(points.get(i))) {
                                for (int k = 0; k < players.size(); k++) {
                                    if (snake.getPlayerId() == players.get(k).getId()) {
                                        System.out.println("[SERVER_MESSAGE] Player " + players.get(k).getName() + " ate yourself");

                                        eatYourself = true;

                                        players.add(createGamePlayer(players.get(k).getName(), players.get(k).getId(), players.get(k).getIpAddress(), players.get(k).getPort(), SnakesProto.NodeRole.VIEWER, 0));

                                        if (players.get(k).getRole() == SnakesProto.NodeRole.DEPUTY) {
                                            for (int s = 0; s < players.size(); s++) {
                                                if (s == k) continue;
                                                if (players.get(s).getRole() == SnakesProto.NodeRole.NORMAL) {
                                                    players.add(createGamePlayer(players.get(s).getName(), players.get(s).getId(), players.get(s).getIpAddress(), players.get(s).getPort(), SnakesProto.NodeRole.DEPUTY, players.get(s).getScore()));
                                                    sendMessage(InetAddress.getByName(players.get(s).getIpAddress()) , players.get(s).getPort(), createChangeRoleRoleMsg(SnakesProto.NodeRole.DEPUTY));
                                                    players.remove(s);
                                                    break;
                                                }
                                            }
                                        } else if (players.get(k).getRole() == SnakesProto.NodeRole.MASTER) {
                                            changeServer = true;
                                        }

                                        players.remove(k);

                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        if (eatYourself) {
                            continue;
                        }

                        boolean eatFood = false;
                        //Сьела ли конкретная змея клетку с едой
                        for (int i = 0; i < food.size(); i++) {
                            if (points.get(0).equals(food.get(i))) {
                                eatFood = true;
                                food.remove(i);
                                break;
                            }
                        }

                        if (eatFood) {
                            for (SnakesProto.GamePlayer player : players) {
                                if (player.getId() == snake.getPlayerId()) {
                                    newPlayers.add(createGamePlayer(player.getName(),
                                            player.getId(), player.getIpAddress(), player.getPort(),
                                            player.getRole(), player.getScore() + 1));

                                }
                            }
                        } else {
                            points.remove(points.size() - 1);
                        }

                        newSnakes.add(createSnake(snake.getPlayerId(), points, snake.getState(),
                                snake.getHeadDirection()));
                    }

                    //проверяем, не столкнулись ли у нас змеи
                    for (int i = 0; i < newSnakes.size(); i++) {
                        for (int j = 0; j < newSnakes.size(); j++) {
                            if (i != j) {
                                for (int k = 1; k < newSnakes.get(j).getPointsList().size(); k++) {
                                    if (newSnakes.get(i).getPointsList().get(0).equals(newSnakes.get(j).getPointsList().get(k))) {
                                        snakesToDelete.add(i);
                                    }
                                }
                            }
                        }
                    }

                    for (int i = 0; i < newSnakes.size(); i++) {
                        for (int j = 0; j < newSnakes.size(); j++) {
                            if (i != j) {
                                if (newSnakes.get(i).getPointsList().get(0).equals(newSnakes.get(j).getPointsList().get(0))) {
                                    boolean deleted = false;

                                    for (Integer integer : snakesToDelete) {
                                        if (integer == i) {
                                            deleted = true;
                                            break;
                                        }
                                    }

                                    if (!deleted) {
                                        snakesToDelete.add(i);
                                        snakesToDelete.add(j);
                                    }
                                }
                            }
                        }
                    }

                    //удаляем змеек, которые столкнулись
                    for (int i = 0; i < newSnakes.size(); i++) {
                        boolean delete = false;
                        for (int j = 0; j < snakesToDelete.size(); j++) {
                            if (i == snakesToDelete.get(j)) {
                                delete = true;
                            }
                        }
                        if (!delete) {
                            finalSnakes.add(newSnakes.get(i));
                        } else {
                            for (int k = 0; k < players.size(); k++) {
                                if (newSnakes.get(i).getPlayerId() == players.get(k).getId()) {
                                    players.add(createGamePlayer(players.get(k).getName(), players.get(k).getId(), players.get(k).getIpAddress(), players.get(k).getPort(), SnakesProto.NodeRole.VIEWER, 0));

                                    if (players.get(k).getRole() == SnakesProto.NodeRole.DEPUTY) {
                                        for (int s = 0; s < players.size(); s++) {
                                            if (s == k) continue;
                                            if (players.get(s).getRole() == SnakesProto.NodeRole.NORMAL) {
                                                players.add(createGamePlayer(players.get(s).getName(), players.get(s).getId(), players.get(s).getIpAddress(), players.get(s).getPort(), SnakesProto.NodeRole.DEPUTY, players.get(s).getScore()));
                                                sendMessage(InetAddress.getByName(players.get(s).getIpAddress()), players.get(s).getPort(), createChangeRoleRoleMsg(SnakesProto.NodeRole.DEPUTY));
                                                players.remove(s);
                                                break;
                                            }
                                        }
                                    } else if (players.get(k).getRole() == SnakesProto.NodeRole.MASTER) {
                                        for (int s = 0; s < players.size(); s++) {
                                            if (s == k) continue;
                                            changeServer = true;
                                        }
                                    }

                                    players.remove(k);

                                    break;
                                }
                            }
                        }
                    }

                    //добавялем остальных игроков, которых мы не добавляли в список новых игроков
                    for (SnakesProto.GamePlayer player : players) {
                        boolean addOrNo = false;
                        for (SnakesProto.GamePlayer newPlayer : newPlayers) {
                            if (player.getId() == newPlayer.getId()) {
                                addOrNo = true;
                                break;
                            }
                        }

                        if (!addOrNo) {
                            newPlayers.add(createGamePlayer(player.getName(),
                                    player.getId(), player.getIpAddress(), player.getPort(),
                                    player.getRole(), player.getScore()));
                        }

                    }

                    snakes.clear();
                    snakes.addAll(finalSnakes);

                    for (SnakesProto.GameState.Snake snake : snakes) {
                        for (SnakesProto.GameState.Coord point : snake.getPointsList()) {
                            gameField.setSnakeCell(point.getY(), point.getX());
                        }
                    }

                    gameField.generateAllFood();

                    players.clear();
                    players.addAll(newPlayers);

                    if (changeServer) {
                        System.out.println("Я умер, поэтому необходимо сменить сервер");
                        for (int s = 0; s < players.size(); s++) {
                            if (players.get(s).getRole() == SnakesProto.NodeRole.DEPUTY) {
                                System.out.println("Отправляю заместителю сообщение о том, что ему необходимо стать сервером");
                                sendMessage(InetAddress.getByName(players.get(s).getIpAddress()) , players.get(s).getPort(), createChangeRole2Msg(SnakesProto.NodeRole.MASTER, SnakesProto.NodeRole.DEPUTY));
                                System.out.println("Отправляю последнее состояние игрового поля на умирающем сервере своему заместител");
                                sendGameStateMessageToEveryone();

                                endServer();
                                break;
                            }
                        }
                    } else {
                        sendGameStateMessageToEveryone();
                    }

                    Thread.sleep(stateDelayMs);
                }
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException ignored) {

            }
        }
    }

    public class ResendClientServerConfirmationTimerTask extends TimerTask {
        private static final long MAX_TIME_SENT = 15;

        @Override
        public void run() {
            List<GameMessageWrapper> messagesToSend = new ArrayList<>();
            try {
                clientUnconfirmedMessagesSem.acquire();
                for (int i = 0; i < unconfirmedMessagesFromClient.size(); ++i) {
                    GameMessageWrapper gameMessageWrapper = unconfirmedMessagesFromClient.get(i);
                    if (gameMessageWrapper.getTimesSent() >= MAX_TIME_SENT) {
                        unconfirmedMessagesFromClient.clear();

                        events.notify("serverDown");

                        if (nodeRole == SnakesProto.NodeRole.DEPUTY) {
                            getSnakes().addAll(save_snakes);
                            getFood().addAll(save_food);
                            getPlayers().addAll(save_players); 

                            startServerLogic();
                            sendNewServerToEveryone();
                        }
                        break;
                    }

                    messagesToSend.add(gameMessageWrapper);

                    System.out.println("Resend client message to the server");

                    gameMessageWrapper.increaseTimesSent();
                }
                clientUnconfirmedMessagesSem.release();

                for (GameMessageWrapper gameMessageWrapper  : messagesToSend) {
                    sendMessage(gameMessageWrapper.getInetAddress(), gameMessageWrapper.getPort(), gameMessageWrapper.getGameMessage());
                }
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    public class ResendServerConfirmationTimerTask extends TimerTask {
        private static final long MAX_TIME_SENT = 2;

        @Override
        public void run() {
            List<GameMessageWrapper> messagesToSend = new ArrayList<>();
            try {
                serverUnconfirmedMessagesSem.acquire();
                Iterator<GameMessageWrapper> it = unconfirmedMessagesFromServer.iterator();
                while (it.hasNext()) {
                    GameMessageWrapper gameMessageWrapper = it.next();
                    if (gameMessageWrapper.getTimesSent() >= MAX_TIME_SENT) {
                        deletePlayer(gameMessageWrapper.getInetAddress().getHostAddress(), gameMessageWrapper.getPort());
                        unconfirmedMessagesFromServer.remove(gameMessageWrapper);
                        break;
                    }

                    messagesToSend.add(gameMessageWrapper);

                    System.out.println("Resend server message to the client");

                    gameMessageWrapper.increaseTimesSent();
                }

                serverUnconfirmedMessagesSem.release();

                for (GameMessageWrapper gameMessageWrapper  : messagesToSend) {
                    sendMessage2(gameMessageWrapper.getInetAddress(), gameMessageWrapper.getPort(), gameMessageWrapper.getGameMessage());
                }
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    public synchronized void sendMessage2(InetAddress inetAddress, int port, SnakesProto.GameMessage gameMessage) throws IOException, InterruptedException {
        byte[] buf = gameMessage.toByteArray();

        datagramSocket.send(new DatagramPacket(buf, buf.length, inetAddress, port));
    }

    public void sendNewServerToEveryone() throws IOException, InterruptedException {
        for (SnakesProto.GamePlayer player : players) {
            if (player.getRole() != SnakesProto.NodeRole.DEPUTY) {
                sendMessage(InetAddress.getByName(player.getIpAddress()), player.getPort(), createChangeRole3Msg());
            }
        }
    }

    private SnakesProto.GameMessage createChangeRole3Msg() {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.RoleChangeMsg.Builder roleChangeMsg = SnakesProto.GameMessage.RoleChangeMsg.newBuilder();
        roleChangeMsg.setReceiverRole(SnakesProto.NodeRole.NORMAL);
        roleChangeMsg.setSenderRole(SnakesProto.NodeRole.MASTER);

        gameMessage.setRoleChange(roleChangeMsg);

        gameMessage.setMsgSeq(server.getServerMsgCount());
        server.increaseServerMsgCount();

        return gameMessage.build();

    }

    private synchronized void sendGameStateMessageToEveryone() throws IOException, InterruptedException {
        for (SnakesProto.GamePlayer player : players) {
            sendMessage(InetAddress.getByName(player.getIpAddress()), player.getPort(), createStateMsg());
        }
    }

    public synchronized void sendMessage(InetAddress inetAddress, int port, SnakesProto.GameMessage gameMessage) throws IOException, InterruptedException {
        byte[] buf = gameMessage.toByteArray();

        if (gameMessage.hasJoin()) {
            clientUnconfirmedMessagesSem.acquire();
            unconfirmedMessagesFromClient.add(new GameMessageWrapper(gameMessage, inetAddress, port));
            clientUnconfirmedMessagesSem.release();
//            System.out.println("[CLIENT_MESSAGE] Client send joinMsg with seq [" + gameMessage.getMsgSeq() + "]");
        } else if (gameMessage.hasPing()) {
            clientUnconfirmedMessagesSem.acquire();
            unconfirmedMessagesFromClient.add(new GameMessageWrapper(gameMessage, inetAddress, port));
            clientUnconfirmedMessagesSem.release();
//            System.out.println("[CLIENT_MESSAGE] Client send pingMsg with seq [" + gameMessage.getMsgSeq() + "]");
        } else if (gameMessage.hasState()) {
            serverUnconfirmedMessagesSem.acquire();
            unconfirmedMessagesFromServer.add(new GameMessageWrapper(gameMessage, inetAddress, port));
            serverUnconfirmedMessagesSem.release();
            System.out.println("[SERVER_MESSAGE] Server send stateMsg with seq [" + gameMessage.getMsgSeq() + "]");
        } else if (gameMessage.hasSteer()) {
            clientUnconfirmedMessagesSem.acquire();
            unconfirmedMessagesFromClient.add(new GameMessageWrapper(gameMessage, inetAddress, port));
            clientUnconfirmedMessagesSem.release();
            System.out.println("[CLIENT_MESSAGE] Client send steerMsg with seq [" + gameMessage.getMsgSeq() + "]");
        } else if (gameMessage.hasAck()) {
//            System.out.println("[SERVER_MESSAGE] Server send ackMsg with seq [" + gameMessage.getMsgSeq() + "]");
        }

        datagramSocket.send(new DatagramPacket(buf, buf.length, inetAddress, port));
    }

    public synchronized void clientConfirmMessageWithSeq(long msgSeq) {
        for (int i = 0; i < unconfirmedMessagesFromClient.size(); i++) {
            if (unconfirmedMessagesFromClient.get(i).getGameMessage().getMsgSeq() == msgSeq) {
                unconfirmedMessagesFromClient.remove(i);
                return;
            }
        }
    }

    public synchronized void serverConfirmMessageWithSeq(long msgSeq) {
        for (int i = 0; i < unconfirmedMessagesFromServer.size(); i++) {
            if (unconfirmedMessagesFromServer.get(i).getGameMessage().getMsgSeq() == msgSeq) {
                unconfirmedMessagesFromServer.remove(i);
                return;
            }
        }
    }

    private void deletePlayer(String ipAddress, int port) {
        for (int i = 0; i < players.size(); i++) {
            if (players.get(i).getIpAddress().equals(ipAddress) && players.get(i).getPort() == port) {
                players.remove(i);
                break;
            }
        }
    }

    public SnakesProto.GameMessage createChangeRoleRoleMsg(SnakesProto.NodeRole nodeRole) {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.RoleChangeMsg.Builder roleChangeMsg = SnakesProto.GameMessage.RoleChangeMsg.newBuilder();
        roleChangeMsg.setReceiverRole(nodeRole);

        gameMessage.setRoleChange(roleChangeMsg);

        gameMessage.setMsgSeq(server.getServerMsgCount());
        server.increaseServerMsgCount();

        return gameMessage.build();

    }

    public SnakesProto.GameMessage createChangeRole2Msg(SnakesProto.NodeRole nodeRoleSender, SnakesProto.NodeRole nodeRoleReceiver) {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.RoleChangeMsg.Builder roleChangeMsg = SnakesProto.GameMessage.RoleChangeMsg.newBuilder();
        roleChangeMsg.setReceiverRole(nodeRoleReceiver);
        roleChangeMsg.setSenderRole(nodeRoleSender);

        gameMessage.setRoleChange(roleChangeMsg);

        gameMessage.setMsgSeq(server.getServerMsgCount());
        server.increaseServerMsgCount();

        return gameMessage.build();

    }

    public SnakesProto.GameMessage createJoinMsg() {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.JoinMsg.Builder joinMessage = SnakesProto.GameMessage.JoinMsg.newBuilder();
        joinMessage.setName(playerName);

        gameMessage.setJoin(joinMessage.build());
        gameMessage.setMsgSeq(getClientServerMsgCount());

        increaseClientServerMsgCount();

        return gameMessage.build();
    }

    private SnakesProto.GameMessage createStateMsg() {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.StateMsg.Builder stateMsg = SnakesProto.GameMessage.StateMsg.newBuilder();

        SnakesProto.GameState.Builder gameState = SnakesProto.GameState.newBuilder();

        gameState.setStateOrder(0);

        for (SnakesProto.GameState.Snake snake : snakes) {
            gameState.addSnakes(snake);
        }

        for (SnakesProto.GameState.Coord food : food) {
            gameState.addFoods(food);
        }

        gameState.setPlayers(createGamePlayers());
        gameState.setConfig(createGameConfig());

        stateMsg.setState(gameState);

        gameMessage.setState(stateMsg.build());

        gameMessage.setMsgSeq(server.getServerMsgCount());
        server.increaseServerMsgCount();

        return gameMessage.build();
    }

    public SnakesProto.GameMessage createSteerMsg(SnakesProto.Direction direction) {
        SnakesProto.GameMessage.Builder gameMessage = SnakesProto.GameMessage.newBuilder();

        SnakesProto.GameMessage.SteerMsg.Builder steerMsg = SnakesProto.GameMessage.SteerMsg.newBuilder();

        steerMsg.setDirection(direction);

        gameMessage.setSteer(steerMsg.build());

        gameMessage.setMsgSeq(getClientServerMsgCount());
        increaseClientServerMsgCount();

        return gameMessage.build();
    }

    public synchronized SnakesProto.GamePlayers createGamePlayers() {
        SnakesProto.GamePlayers.Builder gamePlayers = SnakesProto.GamePlayers.newBuilder();

        for (SnakesProto.GamePlayer player : players) {
            gamePlayers.addPlayers(player);
        }

        return gamePlayers.build();
    }

    private SnakesProto.GamePlayer createGamePlayer(String name, int id, String ipAddress, int port, SnakesProto.NodeRole nodeRole, int score) {
        return SnakesProto.GamePlayer.newBuilder()
                .setName(name)
                .setId(id)
                .setIpAddress(ipAddress)
                .setPort(port)
                .setRole(nodeRole)
                .setType(SnakesProto.PlayerType.HUMAN)
                .setScore(score).build();
    }

    public SnakesProto.GameConfig createGameConfig() {
        SnakesProto.GameConfig.Builder gameConfig = SnakesProto.GameConfig.newBuilder();

        gameConfig.setWidth(getFieldWidth());
        gameConfig.setHeight(getFieldHeight());
        gameConfig.setFoodStatic(getFoodStatic());
        gameConfig.setFoodPerPlayer(getFoodPerPlayer());
        gameConfig.setStateDelayMs(getStateDelayMs());
        gameConfig.setDeadFoodProb(getDeadFoodProb());
        gameConfig.setPingDelayMs(getPingDelayMs());
        gameConfig.setNodeTimeoutMs(getNodeTimeoutMs());

        return gameConfig.build();
    }

    private SnakesProto.GameState.Coord createCoord(int x, int y) {
        SnakesProto.GameState.Coord.Builder coord = SnakesProto.GameState.Coord.newBuilder();

        coord.setX(x);
        coord.setY(y);

        return coord.build();
    }

    private SnakesProto.GameState.Snake createSnake(int playerId, List<SnakesProto.GameState.Coord> points, SnakesProto.GameState.Snake.SnakeState snakeState,
                                                    SnakesProto.Direction direction) {
        SnakesProto.GameState.Snake.Builder snake = SnakesProto.GameState.Snake.newBuilder();

        snake.setPlayerId(playerId);

        for (int i = 0; i < points.size(); i++) {
            snake.addPoints(i, points.get(i));
        }

        snake.setState(snakeState);
        snake.setHeadDirection(direction);

        return snake.build();
    }

    public SnakesProto.Direction findMyOwnSnakeDirection() {
        for (SnakesProto.GamePlayer player : players) {
            if (player.getName().equals(playerName)) {
                for (SnakesProto.GameState.Snake snake : snakes) {
                    if (snake.getPlayerId() == player.getId()) {
                        return snake.getHeadDirection();
                    }
                }
            }
        }
        return null;
    }
}
