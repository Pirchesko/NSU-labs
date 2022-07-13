import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.Dsl;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        try (AsyncHttpClient asyncHttpClientInstance = Dsl.asyncHttpClient()) {
            System.out.print("Введите название искомой локации: ");
            String searchingPositionName = sc.nextLine();

            Future<Position> positionFuture = CompletableFuture
                    .supplyAsync(new PositionsProvider(asyncHttpClientInstance, searchingPositionName))
                    .thenApply(foundPosition -> {
                        System.out.println("Список найденных по вашему запросу мест:");
                        System.out.println("\n----------------------------------------------------\n");
                        int idx = 1;
                        for (Position position : foundPosition) {
                            System.out.println("Номер локации: " + idx);
                            ++idx;
                            System.out.println(position.getPositionDescription());
                        }

                        System.out.print("\nВведите номер нужной вам локации: ");
                        int requiredPositionNumber = sc.nextInt() - 1;

                        System.out.print("Введите радиус поиска интересных мест (в метрах): ");
                        foundPosition.get(requiredPositionNumber).searchRadius = sc.nextInt();

                        System.out.print("Введите максимальное количество интересных мест: ");
                        foundPosition.get(requiredPositionNumber).countPlacesUpperBound = sc.nextInt();

                        System.out.println("\n----------------------------------------------------\n");

                        return foundPosition.get(requiredPositionNumber);
                    })
                    .thenApply(requiredPosition -> {
                        List<CompletableFuture<Void>> tasks = new ArrayList<>();

                        tasks.add(CompletableFuture
                                .runAsync(new GetWeather(asyncHttpClientInstance, requiredPosition)));
                        tasks.add(CompletableFuture
                                .runAsync(new FindSpots(asyncHttpClientInstance, requiredPosition)));

                        CompletableFuture.allOf(tasks.toArray(new CompletableFuture[0])).join();

                        return requiredPosition;
                    });


            Position foundPositionData = positionFuture.get();

            System.out.println(foundPositionData.getCurrentWeather());
            System.out.println(foundPositionData.getInterestingPlacesData());
        } catch (IOException | ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}