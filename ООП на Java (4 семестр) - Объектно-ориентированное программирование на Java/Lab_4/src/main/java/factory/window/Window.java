package factory.window;

import factory.PropertiesFactory;
import factory.TimeSettings;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Window extends JFrame{
    public int accessoriesSize = 1;
    public int accessoriesLimit = 1;
    public int carcassSize = 1;
    public int carcassLimit = 1;
    public int engineSize = 1;
    public int engineLimit = 1;
    public int autoSize = 1;
    public int autoLimit = 1;
    JLabel accessories = new JLabel("accessories");
    JLabel carcass = new JLabel("carcass");
    JLabel engine = new JLabel("engine");
    JLabel auto = new JLabel("auto");
    JButton accessoriesButton = new JButton("Accessories duration: " + TimeSettings.getAccessoriesTime());
    JButton carcassButton = new JButton("Carcass duration: " + TimeSettings.getCarcassTime());
    JButton engineButton = new JButton("Engine duration: " + TimeSettings.getEngineTime());
    JButton workerButton = new JButton("Worker duration: " + TimeSettings.getWorkerTime());
    JButton dealerButton = new JButton("Dealer duration: " + TimeSettings.getDealerTime());
    JLabel info = new JLabel("Some information:");
    JLabel countAccessories = new JLabel("Suppliers of Accessory: ");
    JLabel countCarcass = new JLabel("Suppliers of Carcass: ");
    JLabel countEngine = new JLabel("Suppliers of Engine: ");
    JLabel countWorker = new JLabel("Total Workers: ");
    JLabel countDealer = new JLabel("Total Dealers: ");
    public Window(){
        super("My Factory");
        setSize(300, 425);
        JPanel panel = new JPanel(new VerticalLayout());
        Font font = new Font("Consolas", Font.BOLD, 14);

        accessories.setFont(font);
        carcass.setFont(font);
        engine.setFont(font);
        auto.setFont(font);
        info.setFont(font);
        countAccessories.setFont(font);
        countCarcass.setFont(font);
        countEngine.setFont(font);
        countWorker.setFont(font);
        countDealer.setFont(font);

        accessoriesButton.addActionListener(new ListenerAction());
        carcassButton.addActionListener(new ListenerAction());
        engineButton.addActionListener(new ListenerAction());
        workerButton.addActionListener(new ListenerAction());
        dealerButton.addActionListener(new ListenerAction());

        panel.add(accessories);
        panel.add(accessoriesButton);
        panel.add(carcass);
        panel.add(carcassButton);
        panel.add(engine);
        panel.add(engineButton);
        panel.add(auto);
        panel.add(workerButton);
        panel.add(dealerButton);
        panel.add(info);
        panel.add(countAccessories);
        panel.add(countCarcass);
        panel.add(countEngine);
        panel.add(countWorker);
        panel.add(countDealer);

        getContentPane().add(panel);
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    public void setAccessoriesInfo(int accessoriesS, int accessoriesL){
        accessoriesSize = accessoriesS;
        accessoriesLimit = accessoriesL;
        accessoriesButton.setText("Accessories duration: " + TimeSettings.getAccessoriesTime());
        accessories.setText("accessories:  " + accessoriesSize + "/" + accessoriesLimit + "   (" + (((float)accessoriesSize/(float)accessoriesLimit)*100) + "%)");
    }
    public void setCarcassInfo(int carcassS, int carcassL){
        carcassSize = carcassS;
        carcassLimit = carcassL;
        carcassButton.setText("Carcass duration: " + TimeSettings.getCarcassTime());
        carcass.setText("carcass:  " + carcassSize + "/" + carcassLimit + "  (" + (((float)carcassSize/(float)carcassLimit)*100) + "%)");
    }
    public void setEngineInfo(int engineS, int engineL){
        engineSize = engineS;
        engineLimit = engineL;
        engineButton.setText("Engine duration: " + TimeSettings.getEngineTime());
        engine.setText("engine:  " + engineSize + "/" + engineLimit + "  (" + (((float)engineSize/(float)engineLimit)*100) + "%)");
    }
    public void setAutoInfo(int autoS, int autoL){
        autoSize = autoS;
        autoLimit = autoL;
        auto.setText("auto:  " + autoSize + "/" + autoLimit + "  (" + (((float)autoSize/(float)autoLimit)*100) + "%)");
    }
    public void setWorkerInfo(){
        workerButton.setText("Worker duration: " + TimeSettings.getWorkerTime());
    }
    public void setDealerInfo(){
        dealerButton.setText("Dealer duration: " + TimeSettings.getDealerTime());
    }
    public void setSettingsFactory(PropertiesFactory settingsFactory){
        countAccessories.setText("Suppliers of Accessory: " + settingsFactory.getCountAccessories());
        countCarcass.setText("Suppliers of Carcass: " + settingsFactory.getCountCarcass());
        countEngine.setText("Suppliers of Engine: " + settingsFactory.getCountEngine());
        countWorker.setText("Total Workers: " + settingsFactory.getCountWorkers());
        countDealer.setText("Total Dealers: " + settingsFactory.getCountDealers());
    }

    class VerticalLayout implements LayoutManager {
        private Dimension size = new Dimension();

        // Следующие два метода не используются
        public void addLayoutComponent   (String name, Component comp) {}
        public void removeLayoutComponent(Component comp) {}

        // Метод определения минимального размера для контейнера
        public Dimension minimumLayoutSize(Container c) {
            return calculateBestSize(c);
        }
        // Метод определения предпочтительного размера для контейнера
        public Dimension preferredLayoutSize(Container c) {
            return calculateBestSize(c);
        }
        // Метод расположения компонентов в контейнере
        public void layoutContainer(Container container)
        {
            // Список компонентов
            Component list[] = container.getComponents();
            int currentY = 5;
            for (int i = 0; i < list.length; i++) {
                // Определение предпочтительного размера компонента
                Dimension pref = list[i].getPreferredSize();
                // Размещение компонента на экране
                list[i].setBounds(5, currentY, pref.width, pref.height);
                // Учитываем промежуток в 5 пикселов
                currentY += 5;
                // Смещаем вертикальную позицию компонента
                currentY += pref.height;
            }
        }
        // Метод вычисления оптимального размера контейнера
        private Dimension calculateBestSize(Container c)
        {
            // Вычисление длины контейнера
            Component[] list = c.getComponents();
            int maxWidth = 0;
            for (int i = 0; i < list.length; i++) {
                int width = list[i].getWidth();
                // Поиск компонента с максимальной длиной
                if ( width > maxWidth )
                    maxWidth = width;
            }
            // Размер контейнера в длину с учетом левого отступа
            size.width = maxWidth + 5;
            // Вычисление высоты контейнера
            int height = 0;
            for (int i = 0; i < list.length; i++) {
                height += 5;
                height += list[i].getHeight();
            }
            size.height = height;
            return size;
        }
    }

    class ListenerAction implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            System.out.println("Button clicked: " + e.getActionCommand() + "\n");
            String m = JOptionPane.showInputDialog("Input duration in ms", 1000);
            String[] command = e.getActionCommand().split(" ");
            if (m == null){
                return;
            }
            int time = Integer.parseInt(m);
            if (time >= 100 && time <= 100000){
                switch (command[0]){
                    case("Accessories"):
                        System.out.println("Accessories duration changed from " + TimeSettings.getAccessoriesTime() + " to " + time);
                        TimeSettings.setAccessoriesTime(time);
                        break;
                    case("Carcass"):
                        System.out.println("Carcass duration changed from " + TimeSettings.getCarcassTime() + " to " + time);
                        TimeSettings.setCarcassTime(time);
                        break;
                    case("Engine"):
                        System.out.println("Engine duration changed from " + TimeSettings.getEngineTime() + " to " + time);
                        TimeSettings.setEngineTime(time);
                        break;
                    case("Dealer"):
                        System.out.println("Dealer duration changed from " + TimeSettings.getDealerTime() + " to " + time);
                        TimeSettings.setDealerTime(time);
                        break;
                    case("Worker"):
                        System.out.println("Worker duration changed from " + TimeSettings.getWorkerTime() + " to " + time);
                        TimeSettings.setWorkerTime(time);
                        break;
                }
            }else{
                System.out.println("Bad duration!");
            }
        }
    }
}
