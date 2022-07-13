package socks;

import org.xbill.DNS.Message;
import org.xbill.DNS.Section;

import java.io.IOException;
import java.net.InetAddress;
import java.nio.channels.SelectionKey;

public class DnsUtils {
    public static void read(SelectionKey key) throws IOException {
        //System.out.println(key);

        var attach = SocksUtils.getAttachment(key);

        var message = new Message(attach.getIn().array());
        var maybeRecord = message.getSection(Section.ANSWER).stream().findAny();
        if (maybeRecord.isPresent()) {
            var addr = InetAddress.getByName(maybeRecord.get().rdataToString());
            System.out.println("Resolved: " + maybeRecord.get().rdataToString());

            SocksUtils.getAttachment(attach.getCoupled()).couple(addr, attach.getPort(), attach.getCoupled());
            SocksUtils.getAttachment(attach.getCoupled()).setType(Type.NONE);
            key.interestOps(0);
            SocksUtils.partiallyClose(key);
        } else {
            System.out.println(message.toString());
            SocksUtils.close(key);
            throw new RuntimeException("Host cannot be resolved");
        }
    }

    public static void write(SelectionKey key) {
        //System.out.println(key);
        SocksUtils.clearOut(key);
        key.interestOpsOr(SelectionKey.OP_READ);
        key.interestOpsAnd(~SelectionKey.OP_WRITE);
        SocksUtils.getAttachment(key).setType(Type.DNS_READ);
    }
}
