import java.net.DatagramPacket;
import java.util.ArrayList;
import java.util.HashMap;

import static java.lang.Thread.sleep;

public class Statistic implements Runnable {
    private static int TIMEOUT = 1000;
    private HashMap<String, ArrayList<Integer>> statistic = new HashMap<>();
    private String lastIP;

    public void checkPacket(DatagramPacket packet) {
        String hostIp = packet.getAddress().getHostAddress();
        lastIP = hostIp;
        if (!statistic.containsKey(hostIp)) { //если у нас нет такого ip то создаём массив портов
            ArrayList<Integer> ports = new ArrayList<>();
            ports.add(packet.getPort());
            statistic.put(hostIp, ports);
        } else { //если есть такой ip то добавляем порт
            if(!statistic.get(hostIp).contains(packet.getPort())){ //есть ли у нас такой порт? если нет то добавляем
                statistic.get(hostIp).add(packet.getPort());
            }
        }
    }

    @Override
    public void run() {
        int countIP = 0;
        int countPorts = 0;
        while (!Thread.currentThread().isInterrupted()) {
            try {
                statistic.clear();
                sleep(TIMEOUT);
                if ((countIP != statistic.size()) || (statistic.get(lastIP).size() != countPorts)){
                    countIP = statistic.size();
                    countPorts = statistic.get(lastIP).size();
                    System.out.println(statistic.toString() + "\n");
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
