import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.Response;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

public class PositionsProvider implements Supplier<List<Position>> {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final String searchingLocationName;

    public PositionsProvider(AsyncHttpClient asyncHttpClientInstance,
                             String searchingLocationName) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.searchingLocationName = searchingLocationName;
    }

    @Override
    public List<Position> get() {
        String url = "https://graphhopper.com/api/1/geocode?q=" + searchingLocationName + "&locale=en&key=21e4d1d9-226e-45ec-b038-ab0405844186";

        CompletableFuture<Response> getResponse = asyncHttpClientInstance.prepareGet(url).execute().toCompletableFuture();

        List<Position> foundPositions = getResponse.thenApply(response -> {
            JSONObject parsedResponse;
            try {
                parsedResponse = (JSONObject) new JSONParser().parse(response.getResponseBody());
            } catch (ParseException e) {
                e.printStackTrace();
                System.err.println("\n\tPARSE_ERROR (Locations JSON)!!!\n");
                return null;
            }

            JSONArray hits = (JSONArray) parsedResponse.get("hits");
            ArrayList<Position> positions = new ArrayList<>();

            for (var point : hits) {
                Position nextFoundPosition = new Position();

                nextFoundPosition.latitude =
                        ((JSONObject) ((JSONObject) point).get("point")).get("lat").toString();
                nextFoundPosition.longitude =
                        ((JSONObject) ((JSONObject) point).get("point")).get("lng").toString();
                nextFoundPosition.positionName =
                        ((JSONObject) point).get("name").toString();

                nextFoundPosition.city = ((JSONObject) point).get("city") == null ? null
                        : ((JSONObject) point).get("city").toString();

                nextFoundPosition.street = ((JSONObject) point).get("street") == null ? null
                        : ((JSONObject) point).get("street").toString();

                nextFoundPosition.country = ((JSONObject) point).get("country") == null ? null
                        : ((JSONObject) point).get("country").toString();

                positions.add(nextFoundPosition);
            }
            return positions;

        }).join();

        return foundPositions;
    }
}