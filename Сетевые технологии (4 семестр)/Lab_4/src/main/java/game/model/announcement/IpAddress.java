package game.model.announcement;

public class IpAddress {
    private String ip;
    private long time;

    public IpAddress(String ip) {
        this.ip = ip;
        updateTime();
    }

    public String getIp() { return this.ip; }

    public long getTime() { return this.time; }

    public void updateTime() { this.time = System.currentTimeMillis(); }
}
