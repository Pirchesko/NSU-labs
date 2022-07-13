import java.util.ArrayList;
import java.util.List;

public final class Position {

    String latitude;
    String longitude;
    String positionName;
    String country;
    String city;
    String street;

    List<Spot> interestingSpots = new ArrayList<>();
    int searchRadius;
    int countPlacesUpperBound;

    String currentTemperature;
    String currentWindSpeed;
    String weatherDescription;

    public String getPositionDescription() {
        return "Название места: " + positionName + "\n\tСтрана: " + country + "\n\tГород: " + city + "\n\tУлица: " + street + "\n\tДолгота: " + longitude + "\n\tширота: " + latitude;
    }

    public String getCurrentWeather() {
        return "Погода в этом месте:" + "\n\tТемперптура: " + currentTemperature + "\n\tСкорость ветра: " + currentWindSpeed + "\n\tОписание погоды: " + weatherDescription + "\n";
    }

    public String getInterestingPlacesData() {
        StringBuilder sb = new StringBuilder();
        sb.append("Список интересных мест в радиусе ")
                .append(searchRadius)
                .append(" местров с описанием:\n");
        for (Spot spot : interestingSpots) {
            sb.append('\t');
            sb.append("Название места: ");
            sb.append(spot.name);
            sb.append("\n\txID: ");
            sb.append(spot.xid);
            sb.append("\n\tОписание: ");
            sb.append(spot.description);
            sb.append("\n\n");
        }
        return sb.toString();
    }

}