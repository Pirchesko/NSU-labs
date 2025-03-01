package socks;

import org.xbill.DNS.*;
import lombok.RequiredArgsConstructor;
import org.xbill.DNS.Record;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.channels.*;

@RequiredArgsConstructor
public class TinyServer {
    private final Integer port;

    /**
     * runs until stopped
     */
    public void start() throws IOException {
        Selector selector = Selector.open();

        ServerSocketChannel server = ServerSocketChannel.open();
        server.bind(new InetSocketAddress("localhost", port));
        server.configureBlocking(false);
        server.register(selector, SelectionKey.OP_ACCEPT);


        while (true) {
            selector.select();
            //System.out.println(selector.keys());
            var iterator = selector.selectedKeys().iterator();
            while (iterator.hasNext()) {
                var key = iterator.next();
                iterator.remove();

                if (key.isValid()) {
                    try {
                        if (key.isAcceptable()) {
                            handleAccept(key);
                        } else if (key.isConnectable()) {
                            handleConnect(key);
                        } else if (key.isReadable()) {
                            handleRead(key);
                        } else if (key.isWritable()) {
                            handleWrite(key);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        SocksUtils.close(key);
                    }
                }
            }
        }
    }

    private void handleAccept(SelectionKey key) throws IOException {
        //System.out.println(key);
        var channel = ((ServerSocketChannel) key.channel()).accept();
        channel.configureBlocking(false);
        channel.register(key.selector(), SelectionKey.OP_READ);
    }

    private void handleConnect(SelectionKey key) throws IOException {
        //System.out.println(key);
        SocksUtils.getChannel(key).finishConnect();
        SocksUtils.getAttachment(key).finishCouple();
        key.interestOps(0);
    }

    private void handleRead(SelectionKey key) throws Exception {
        var attach = SocksUtils.getAttachment(key);
        if(attach == null){
            prepareAttachment(key);
        }else{
            if(!SocksUtils.tryReadToBuffer(key)){
                SocksUtils.close(key);
                if(attach.getType() == Type.DNS_READ){
                    throw new Exception("Bad dns reply");
                }
            }
            switch (attach.getType()){
                case AUTH_READ -> replySocksAuth(key);
                case CONN_READ -> replySocksConn(key);
                case DNS_READ -> DnsUtils.read(key);
                default -> prepareCoupledWrite(key);
            }
        }
    }

    private void handleWrite(SelectionKey key) throws IOException {
        var attach = SocksUtils.getAttachment(key);
        if(!SocksUtils.tryWriteToBuffer(key)){
            SocksUtils.close(key);
        }else if( SocksUtils.outIsEmpty(key)) {
            switch (attach.getType()) {
                case AUTH_WRITE -> authWrite(key);
                case DNS_WRITE -> DnsUtils.write(key);
                default -> {
                    if(SocksUtils.getAttachment(key).isDecoupled()){
                        SocksUtils.close(key);
                    }else {
                        prepareCoupledRead(key);
                    }
                }
            }
        }
    }

    private void prepareAttachment(SelectionKey key) {
        //System.out.println(key);
        key.attach(new Attachment(Type.AUTH_READ));
    }

    private void prepareCoupledWrite(SelectionKey key) throws IOException {
        if(SocksUtils.isDecoupled(key)){
            SocksUtils.close(key);
            return;
        }
        // couple exists 'cause of conn earlier in switch
        // disable read + enable coupled read
        //System.out.println(key);
        SocksUtils.getAttachment(key).addCoupledWrite();
        key.interestOps(key.interestOps() ^ SelectionKey.OP_READ);
    }

    private void prepareCoupledRead(SelectionKey key) throws IOException {
        if(SocksUtils.isDecoupled(key)){
            SocksUtils.close(key);
            return;
        }
        // disable write + enable coupled read
        //System.out.println(key);
        SocksUtils.clearOut(key);
        SocksUtils.getAttachment(key).addCoupledRead();
        key.interestOps(key.interestOps() ^ SelectionKey.OP_WRITE);
    }

    private void authWrite(SelectionKey key) {
        //System.out.println(key);
        SocksUtils.clearOut(key);
        key.interestOps(SelectionKey.OP_READ);
        SocksUtils.getAttachment(key).setType(Type.CONN_READ);
    }

    private void replySocksConn(SelectionKey key) throws Exception {
        //System.out.println(key);
        if(SocksMessage.inTooSmall(key, SocksMessage.CONN_SMALLNESS)){
            System.out.println("Connection: buffer too small; waiting");
            return;
        }
        if(SocksMessage.wrongVersion(key)){
            // yea i hate other users of proxy, wdya
            throw new Exception("Wrong version, SOCKS5 expected");
        }
        if(SocksMessage.wrongCommand(key)){
            // yea i hate other users of proxy, wdya
            throw new Exception("Wrong command, 0x01 expected");
        }
        if(SocksMessage.wrongAddrType(key)){
            // yea i hate other users of proxy, wdya
            throw new Exception("Wrong addrtype, ipv4 or host expected");
        }

        if(SocksMessage.isIpv4(key)){
            var addr = InetAddress.getByAddress(SocksMessage.getIpv4(key));
            var port = SocksMessage.getPort(key, SocksMessage.IP_PORTPOS);

            SocksUtils.couple(addr, port, key);
            key.interestOps(0);
        } else
            if(SocksMessage.isHost(key)){
                if(SocksMessage.inTooSmall(key, SocksMessage.HOST_SMALLNESS)) {
                    System.out.println("Hostname connection: buffer too small; waiting");
                    return;
                }
                //TODO if too much?
                var hostname = SocksMessage.getHost(key);
                var port = SocksMessage.getPort(key, SocksMessage.HOST_PORTPOS);
                System.out.println("resolving '" + hostname + "' on " + port);

                key.interestOps(0);

                registerHostResolve(hostname, port, key);
            }

    }

    private void registerHostResolve(String hostname, int port, SelectionKey key) throws IOException {
        //System.out.println(key); ???
        var dnsChan = DatagramChannel.open();
        dnsChan.connect(ResolverConfig.getCurrentConfig().server());
        dnsChan.configureBlocking(false);

        var dnsKey = dnsChan.register(key.selector(), SelectionKey.OP_WRITE);
        var dnsAttach = new Attachment(port, key, Type.DNS_WRITE);

        var message = new Message();
        var record = Record.newRecord(Name.fromString(hostname + '.').canonicalize(), org.xbill.DNS.Type.A, DClass.IN);
        message.addRecord(record, Section.QUESTION);

        var header = message.getHeader();
        header.setFlag(Flags.AD);
        header.setFlag(Flags.RD);

        dnsAttach.getIn().put(message.toWire());
        dnsAttach.useInAsOut();

        dnsKey.attach(dnsAttach);

        System.out.println("Registered DNS resolve");
    }

    private void replySocksAuth(SelectionKey key) throws Exception {
        //System.out.println(key);
        var at = SocksUtils.getAttachment(key);

        if(SocksMessage.inTooSmall(key, SocksMessage.AUTH_SMALLNESS)){
            System.out.println("Authentication: buffer too small; waiting");
            return;
        }
        if(SocksMessage.wrongVersion(key)){
            // yea i hate other users of proxy, wdya
            throw new Exception("Wrong version, SOCKS5 expected");
        }
        if(!SocksMessage.methodsReceived(key)){
            System.out.println("Authentication: waiting for additional methods");
            return;
        }
        if(SocksMessage.noAuthNotFound(key)){
            // hate intensifies
            throw new Exception("auth: noAuth method not found");
        }

        SocksMessage.putNoAuthResponse(key);
        SocksUtils.getAttachment(key).setType(Type.AUTH_WRITE);
        key.interestOps(SelectionKey.OP_WRITE);
    }

}
