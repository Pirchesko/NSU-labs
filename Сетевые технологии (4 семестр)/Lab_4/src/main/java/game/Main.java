package game;

public class Main {
    public static int port;

    public static void main(String[] args) {
        port = Integer.parseInt(args[0]);
        MainFX.main(args);
    }
}
