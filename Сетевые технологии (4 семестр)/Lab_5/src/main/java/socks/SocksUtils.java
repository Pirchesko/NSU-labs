package socks;

import java.io.IOException;
import java.net.InetAddress;
import java.nio.channels.ByteChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

public class SocksUtils {
    public static Attachment getAttachment(SelectionKey key){
        return (Attachment) key.attachment();
    }

    public static SocketChannel getChannel(SelectionKey key){
        return (SocketChannel) key.channel();
    }
    public static ByteChannel getByteChannel(SelectionKey key){
        return (ByteChannel) key.channel();
    }

    public static void clearIn(SelectionKey key){
        getAttachment(key).getIn().clear();
    }

    public static void clearOut(SelectionKey key){
        getAttachment(key).getOut().clear();
    }

    public static boolean isDecoupled(SelectionKey key){
        return getAttachment(key).isDecoupled();
    }

    public static boolean tryReadToBuffer(SelectionKey key) throws IOException {
        var res = getByteChannel(key).read(getAttachment(key).getIn());
        System.out.println("Read in buffer: " + res);
        return (res > 0);
    }

    public static void useInAsOut(SelectionKey key) {
        getAttachment(key).setOut(getAttachment(key).getIn());
    }

    public static void couple(InetAddress addr, int port, SelectionKey key) throws IOException {
        getAttachment(key).couple(addr, port, key);
    }

    public static void close(SelectionKey key) throws IOException {
        //System.out.println(key);
        key.cancel();
        key.channel().close();
        SocksUtils.getAttachment(key).decouple();
    }

    public static void partiallyClose(SelectionKey key) throws IOException {
        key.cancel();
        key.channel().close();
    }

    public static boolean tryWriteToBuffer(SelectionKey key) throws IOException {
        //System.out.println(key.channel());
        var buf = getAttachment(key).getOut();
        buf.flip();
        var res = getByteChannel(key).write(buf);
        System.out.println("Write in buffer: " + res);
        return (res > 0);
    }

    public static boolean outIsEmpty(SelectionKey key) {
        return (getAttachment(key).getOut().remaining() == 0);
    }
}
