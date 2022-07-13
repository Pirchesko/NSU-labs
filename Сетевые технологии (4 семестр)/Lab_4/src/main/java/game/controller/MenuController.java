package game.controller;

import game.model.GameModel;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;

public class MenuController {
    private GameModel gameModel = GameModel.getInstance();

    private Image movedStartButton = new Image("game/buttons/new_game_button/new_game_2.png");
    private Image exitedStartButton = new Image("game/buttons/new_game_button/new_game_1.png");

    private Image movedConnectButton = new Image("game/buttons/connect_button/connect_2.png");
    private Image exitedConnectButton = new Image("game/buttons/connect_button/connect_1.png");

    private Image movedOptionsButton = new Image("game/buttons/options_button/options_2.png");
    private Image exitedOptionsButton = new Image("game/buttons/options_button/options_1.png");

    private Image movedExitButton = new Image("game/buttons/exit_button/exit_2.png");
    private Image exitedExitButton = new Image("game/buttons/exit_button/exit_1.png");

    @FXML
    private ImageView newGameButton;

    @FXML
    private ImageView connectButton;

    @FXML
    private ImageView optionsButton;

    @FXML
    private ImageView exitButton;

    public MenuController() throws IOException {
    }

    public void newGameButtonPressed(MouseEvent event) throws IOException {
        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/new_game.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void newGameButtonMoved() {
        newGameButton.setImage(movedStartButton);
    }

    public void newGameButtonExited() {
        newGameButton.setImage(exitedStartButton);
    }

    public void connectButtonPressed(MouseEvent event) throws IOException {
        gameModel.receiveAnnouncementMessages();

        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/connect.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void connectButtonMoved() {
        connectButton.setImage(movedConnectButton);
    }

    public void connectButtonExited() {
        connectButton.setImage(exitedConnectButton);
    }

    public void optionsButtonPressed(MouseEvent event) throws IOException {
        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/options.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void optionsButtonMoved() {
        optionsButton.setImage(movedOptionsButton);
    }

    public void optionsButtonExited() {
        optionsButton.setImage(exitedOptionsButton);
    }

    public void exitButtonPressed(MouseEvent event) {
        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.close();
    }

    public void exitButtonMoved() {
        exitButton.setImage(movedExitButton);
    }

    public void exitButtonExited() {
        exitButton.setImage(exitedExitButton);
    }
}
