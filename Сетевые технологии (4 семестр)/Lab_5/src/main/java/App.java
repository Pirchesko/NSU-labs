import socks.TinyServer;

import java.io.IOException;

public class App
{
    private static final Integer MIN_PORT = 1024;
    private static final Integer MAX_PORT = 65535;

    private static Integer parsePort(String portString){
        var port = Integer.parseInt(portString);
        if(port < MIN_PORT || port > MAX_PORT){
            System.out.println("Incorrect port");
            System.exit(-1);
        }
        return port;
    }

    public static void main( String[] args ) throws IOException {
        if(args.length < 1){
            System.out.println("TinySOCKSServer args : port");
            System.exit(-1);
        }
        var port = parsePort(args[0]);

        System.out.println("Tiny server starting on " + port);
        TinyServer server = new TinyServer(port);
        server.start();
    }
}
