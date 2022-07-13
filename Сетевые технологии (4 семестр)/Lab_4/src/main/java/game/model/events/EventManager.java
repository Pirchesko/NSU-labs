package game.model.events;

import java.net.DatagramPacket;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

import game.model.announcement.IpAddress;
import game.model.SnakesProto.GameMessage.*;
import game.model.SnakesProto.GamePlayer;
import game.model.SnakesProto.GameState.Snake;
import game.model.SnakesProto.GameState.Coord;


public class EventManager {
    private Map<String, Set<EventListener>> listeners = new HashMap<>();

    public EventManager(String... operations) {
        for (String operation : operations) {
            listeners.put(operation, ConcurrentHashMap.newKeySet());
        }
    }

    public void subscribe(EventListener listener, String... eventTypes) {
        for (String eventType : eventTypes) {
            listeners.get(eventType).add(listener);
        }
    }

    public void unsubscribe(EventListener listener, String... eventTypes) {
        for (String eventType : eventTypes) {
            listeners.get(eventType).remove(listener);
        }
    }

    public void unsubscribeAll(String... eventTypes) {
        for (String eventType : eventTypes) {
            listeners.get(eventType).clear();
        }
    }

    public void notify(String eventType,  Map<IpAddress, AnnouncementMsg> availableGames) {
        Set<EventListener> gamesListeners = listeners.get(eventType);
        for (EventListener listener : gamesListeners) {
            listener.update(eventType, availableGames);
        }
    }

    public void notify(String eventType, List<Snake> snakes, List<Coord> food, List<GamePlayer> players) {
        Set<EventListener> gameStateListeners = listeners.get(eventType);
        for (EventListener listener : gameStateListeners) {
            listener.update(eventType, snakes, food, players);
        }
    }

    public void notify(String eventType, DatagramPacket receivedPacket) {
        Set<EventListener> eventListeners = listeners.get(eventType);
        for (EventListener listener : eventListeners) {
            listener.update(eventType, receivedPacket);
        }
    }

    public void notify(String eventType, String message) {
        Set<EventListener> eventListeners = listeners.get(eventType);
        for (EventListener listener : eventListeners) {
            listener.update(eventType, message);
        }
    }

    public void notify(String eventType) {
        Set<EventListener> eventListeners = listeners.get(eventType);
        for (EventListener listener : eventListeners) {
            listener.update(eventType);
        }
    }
}