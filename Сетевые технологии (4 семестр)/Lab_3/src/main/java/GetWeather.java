import org.asynchttpclient.AsyncHttpClient;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;


public class GetWeather implements Runnable {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final Position requiredPosition;

    GetWeather(AsyncHttpClient asyncHttpClientInstance,
               Position requiredPosition) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.requiredPosition = requiredPosition;
    }

    @Override
    public void run() {
        String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + requiredPosition.latitude +
                "&lon=" + requiredPosition.longitude + "&apikey=d9cc4eb99b79ce7cbcd85ca973cb740a";

        asyncHttpClientInstance.prepareGet(url).execute().toCompletableFuture()
                .thenAccept(response -> {

                    JSONObject parsedResponse;
                    try {
                        parsedResponse = (JSONObject) new JSONParser().parse(response.getResponseBody());
                    } catch (ParseException | NumberFormatException e) {
                        e.printStackTrace();
                        System.err.println("\n\tPARSE_ERROR (Weather JSON) !!!\n");
                        return;
                    }

                    JSONArray weatherArray = (JSONArray) (parsedResponse.get("weather"));

                    requiredPosition.currentTemperature =
                            ((JSONObject) parsedResponse.get("main")).get("temp") == null ? null
                                    : ((JSONObject) parsedResponse.get("main")).get("temp").toString();

                    requiredPosition.currentWindSpeed =
                            ((JSONObject) parsedResponse.get("wind")).get("speed") == null ? null
                                    : ((JSONObject) parsedResponse.get("wind")).get("speed").toString();

                    requiredPosition.weatherDescription =
                            ((JSONObject) weatherArray.get(0)).get("description") == null ? null
                                    : ((JSONObject) weatherArray.get(0)).get("description").toString();
                }).join();
    }
}