import org.asynchttpclient.AsyncHttpClient;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

public final class FindSpots implements Runnable {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final Position requiredPosition;
    private static final String apiKey = "5ae2e3f221c38a28845f05b69e1ef792435c955768d705b9735bd4bc";


    FindSpots(AsyncHttpClient asyncHttpClientInstance,
              Position requiredPosition) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.requiredPosition = requiredPosition;
    }

    private CompletableFuture<List<Spot>> findPlacesByXid() {
        String findPlacesUrl = "https://api.opentripmap.com/0.1/ru/places/radius?radius="
                + requiredPosition.searchRadius
                + "&lon=" + requiredPosition.longitude
                + "&lat=" + requiredPosition.latitude
                + "&format=json"
                + "&apikey=" + apiKey;

        return asyncHttpClientInstance.prepareGet(findPlacesUrl).execute()
                .toCompletableFuture()
                .thenApply(response -> {
                    JSONArray parsedResponse = null;
                    try {
                        parsedResponse = (JSONArray) new JSONParser().parse(response.getResponseBody());
                    } catch (ParseException e) {
                        e.printStackTrace();
                        System.err.println("\n\tPARSE_ERROR (Places_1 JSON) !!!\n");
                        System.exit(23);
                    }

                    List<Spot> spots = new ArrayList<>();
                    int countPLaces = 0;
                    for (var place : parsedResponse) {
                        if (countPLaces >= requiredPosition.countPlacesUpperBound) {
                            break;
                        }
                        countPLaces++;

                        Spot nextFoundSpot = new Spot();
                        nextFoundSpot.xid = ((JSONObject) place).get("xid").toString();
                        nextFoundSpot.name = ((JSONObject) place).get("name").toString();

                        if (nextFoundSpot.name.isEmpty()) {
                            continue;
                        }
                        spots.add(nextFoundSpot);
                    }
                    return spots;
                });
    }

    private void findPlacesDescriptions(List<Spot> foundSpots) {
        String urlPrefix = "https://api.opentripmap.com/0.1/ru/places/xid/";
        String urlPostfix = "?apikey=" + apiKey;
        List<CompletableFuture<Void>> findPlacesDescriptionsAsyncRequests = new ArrayList<>();

        for (Spot spot : foundSpots) {
            findPlacesDescriptionsAsyncRequests.add(asyncHttpClientInstance
                    .prepareGet(urlPrefix + spot.xid + urlPostfix)
                    .execute()
                    .toCompletableFuture()
                    .thenAcceptAsync(response -> {
                        JSONObject parsedResponse;
                        try {
                            parsedResponse = (JSONObject) new JSONParser()
                                    .parse(response.getResponseBody());
                        } catch (ParseException e) {
                            e.printStackTrace();
                            System.err.println("\n\tPARSE_ERROR (Places_2 JSON) !!!\n");
                            return;
                        }

                        if (parsedResponse.get("wikipedia_extracts") != null
                                && ((JSONObject) parsedResponse.get("wikipedia_extracts")).get("text") != null) {
                            spot.description = ((JSONObject) parsedResponse.get("wikipedia_extracts"))
                                    .get("text").toString();
                        }
                    }));
        }
        CompletableFuture.allOf(findPlacesDescriptionsAsyncRequests.toArray(new CompletableFuture[0])).join();
    }

    @Override
    public void run() {
        List<Spot> foundSpots = findPlacesByXid().join();
        findPlacesDescriptions(foundSpots);
        requiredPosition.interestingSpots = foundSpots;
    }
}