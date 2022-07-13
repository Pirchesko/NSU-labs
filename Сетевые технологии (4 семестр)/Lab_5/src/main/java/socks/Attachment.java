package socks;

import lombok.Getter;
import lombok.Setter;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

enum Type {
    NONE,
    CONN_READ,
    CONN_WRITE,
    DNS_READ,
    DNS_WRITE,
    AUTH_READ,
    AUTH_WRITE
}

@Getter
@Setter
class Attachment {
    private final static int BUFFER_CAPACITY = 4096;
    private Type type;
    private ByteBuffer in;
    private ByteBuffer out;
    private SelectionKey coupled;

    private int port; // for after dns resolving

    public Attachment(){
        this.in = ByteBuffer.allocate(BUFFER_CAPACITY);
    }

    public Attachment(Type type){
        this();
        this.type = type;
    }

    public Attachment(int port, SelectionKey key, Type type){
        this(type);
        this.port = port;
        this.coupled = key;
    }

    public void decouple() {
        //System.out.println(this);
        var sendoff = this.coupled;
        if (sendoff != null) {
            this.coupled = null;
            if(sendoff.isValid()) {
                sendoff.interestOps(SelectionKey.OP_WRITE);
            }
        }
    }

    public void couple(InetAddress connectAddr, int connectPort, SelectionKey parentKey) throws IOException {
        System.out.println("Coupling on " + connectAddr.toString() + connectPort);

        var coupleChannel = SocketChannel.open();
        coupleChannel.configureBlocking(false);
        var isConnected = coupleChannel.connect(new InetSocketAddress(connectAddr, connectPort));

        this.coupled = coupleChannel.register(parentKey.selector(), SelectionKey.OP_CONNECT);

        Attachment coupledAttachment = new Attachment();
        coupledAttachment.setCoupled(parentKey);
        this.coupled.attach(coupledAttachment);

        this.in.clear();

        if(isConnected) {
            System.out.println("Finished immediately");
            coupledAttachment.finishCouple();
        }
    }

    public void finishCouple(){
        //System.out.println(this);
        SocksMessage.putConnResponse(this.in);
        var coupledAttachment = SocksUtils.getAttachment(this.coupled);
        this.out = coupledAttachment.getIn();
        coupledAttachment.setOut(this.in);
        coupled.interestOps(SelectionKey.OP_READ | SelectionKey.OP_WRITE);
    }

    public void addCoupledWrite() {
        coupled.interestOpsOr(SelectionKey.OP_WRITE);
        SocksUtils.getAttachment(coupled).setType(Type.NONE);
        type = Type.NONE;
    }

    public void addCoupledRead() {
        coupled.interestOpsOr(SelectionKey.OP_READ);
        SocksUtils.getAttachment(coupled).setType(Type.NONE);
        type = Type.NONE;
    }

    public boolean isDecoupled(){
        return (coupled == null);
    }

    public void useInAsOut(){
        this.out = this.in;
    }
}
