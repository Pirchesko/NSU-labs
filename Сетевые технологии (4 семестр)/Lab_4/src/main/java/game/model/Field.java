package game.model;



import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Field {
    private GameModel gameModel = GameModel.getInstance();

    private int fieldHeight;
    private int fieldWidth;

    private int foodNumber;

    public enum Cell {
        EMPTY,
        SNAKE,
        FOOD
    }

    private Cell[][] cells;

    public Field() throws IOException {
        fieldHeight = gameModel.getFieldHeight();
        fieldWidth = gameModel.getFieldWidth();

        cells = new Cell[fieldHeight][fieldWidth];

        for (int i = 0; i < fieldHeight; i++) {
            for (int j = 0; j < fieldWidth; j++) {
                cells[i][j] = Cell.EMPTY;
            }
        }
    }


    public void setEmptyCell(int x, int y) {
        cells[x][y] = Cell.EMPTY;
    }

    public void setSnakeCell(int x, int y) {
        cells[x][y] = Cell.SNAKE;
    }

    public List<SnakesProto.GameState.Coord> findSnakeInitPlace() {
        ArrayList<SnakesProto.GameState.Coord> initCoords = new ArrayList<>();

        int count = 0;
        int i, j;

        for (i = 0; i < fieldHeight; i++) {
            for (j = 0; j < fieldWidth; j++) {
                first:
                {
                    for (int k = i - 2; k < i + 3; k++) {
                        for (int p = j - 2; p < j + 3; p++) {
                            if (p < 0 || k < 0 || p > fieldWidth || k > fieldHeight) {
                                count++;
                                continue;
                            }

                            if (cells[k][p] != Cell.SNAKE) {
                                count++;
                                if (count == 25) {
                                    cells[i][j + 1] = Cell.SNAKE;
                                    cells[i][j] = Cell.SNAKE;

                                    initCoords.add(SnakesProto.GameState.Coord.newBuilder().setY(i).setX(j + 1).build());
                                    initCoords.add(SnakesProto.GameState.Coord.newBuilder().setY(i).setX(j).build());


                                    return initCoords;
                                }
                            } else {
                                count = 0;
                                break first;
                            }
                        }
                    }
                }
            }
            count = 0;
        }
        return null;
    }

     private void calculateFoodNumber() {
        int playerInGame = 0;

        for (SnakesProto.GamePlayer player : gameModel.getPlayers()) {
            if (player.getRole() != SnakesProto.NodeRole.VIEWER) {
                playerInGame++;
            }
        }

        foodNumber = (int) (gameModel.getFoodStatic() + (playerInGame * gameModel.getFoodPerPlayer()));
    }

    public void generateAllFood() {
        calculateFoodNumber();

        boolean isPlace = false;

        for (int i = 0; i < fieldHeight; i++) {
            for (int j = 0; j < fieldWidth; j++) {
                if (cells[i][j] == Cell.EMPTY) {
                    isPlace = true;
                    break;
                }
            }
        }

        if (isPlace) {
            while (gameModel.getFood().size() < foodNumber) {
                if (gameModel.getFood().size() < foodNumber) {
                    int x = (int) (Math.random() * gameModel.getFieldWidth());
                    int y = (int) (Math.random() * gameModel.getFieldHeight());
                    if (cells[y][x] == Cell.EMPTY) {
                        cells[y][x] = Cell.FOOD;
                        gameModel.getFood().add(createCoord(x, y));
                    }
                }
            }
        }

        if (foodNumber == gameModel.getFoodStatic()) {
            while (gameModel.getFood().size() != foodNumber) {
                int x = gameModel.getFood().get(gameModel.getFood().size() - 1).getX();
                int y = gameModel.getFood().get(gameModel.getFood().size() - 1).getY();

                cells[y][x] = Cell.EMPTY;

                gameModel.getFood().remove(gameModel.getFood().size() - 1);
            }
        }
    }

    private SnakesProto.GameState.Coord createCoord(int x, int y) {
        SnakesProto.GameState.Coord.Builder coord = SnakesProto.GameState.Coord.newBuilder();

        coord.setX(x);
        coord.setY(y);

        return coord.build();
    }
}
