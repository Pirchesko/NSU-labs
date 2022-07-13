package game.controller;

import game.model.GameModel;
import game.model.exceptions.WrongArgException;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.InetAddress;
import java.net.URL;
import java.util.ResourceBundle;

public class NewGameController implements Initializable{
    private GameModel gameModel = GameModel.getInstance();

    private Alert alert;

    private Image movedStartButton = new Image("game/buttons/start_button/start_2.png");
    private Image exitedStartButton = new Image("game/buttons/start_button/start_1.png");

    private Image movedBackButton = new Image("game/buttons/back_button/back_2.png");
    private Image exitedBackButton = new Image("game/buttons/back_button/back_1.png");

    @FXML
    private ImageView startButton;

    @FXML
    private ImageView backButton;

    @FXML
    private TextField sizeField1;

    @FXML
    private TextField sizeField2;

    @FXML
    private TextField foodStatic;

    @FXML
    private TextField foodPerPlayer;

    @FXML
    private TextField stateDelayMs;

    @FXML
    private TextField deadFoodProb;

    @FXML
    private TextField pingDelayMs;

    @FXML
    private TextField nodeTimeoutMs;

    @FXML
    private TextField maxPlayersField;

    public NewGameController() throws IOException {
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        alert = new Alert(Alert.AlertType.WARNING);
        alert.setHeaderText(null);
        alert.setTitle("Warning");

        sizeField1.setText(Integer.toString(gameModel.getFieldHeight()));
        sizeField2.setText(Integer.toString(gameModel.getFieldWidth()));
        foodStatic.setText(Integer.toString(gameModel.getFoodStatic()));
        foodPerPlayer.setText(Float.toString(gameModel.getFoodPerPlayer()));
        stateDelayMs.setText(Integer.toString(gameModel.getStateDelayMs()));
        deadFoodProb.setText(Float.toString(gameModel.getDeadFoodProb()));
        pingDelayMs.setText(Integer.toString(gameModel.getPingDelayMs()));
        nodeTimeoutMs.setText(Integer.toString(gameModel.getNodeTimeoutMs()));
        maxPlayersField.setText(Integer.toString(gameModel.getMaxPlayers()));
    }

    public void startButtonPressed(MouseEvent event) throws IOException {
        try {
            gameModel.setFieldHeight(Integer.parseInt(sizeField1.getText()));
            gameModel.setFieldWidth(Integer.parseInt(sizeField2.getText()));
            gameModel.setFoodStatic(Integer.parseInt(foodStatic.getText()));
            gameModel.setFoodPerPlayer(Float.parseFloat(foodPerPlayer.getText()));
            gameModel.setStateDelayMs(Integer.parseInt(stateDelayMs.getText()));
            gameModel.setDeadFoodProb(Float.parseFloat(deadFoodProb.getText()));
            gameModel.setPingDelayMs(Integer.parseInt(pingDelayMs.getText()));
            gameModel.setNodeTimeoutMs(Integer.parseInt(nodeTimeoutMs.getText()));
            gameModel.setMaxPlayers(Integer.parseInt(maxPlayersField.getText()));

            gameModel.startServerLogic();

            gameModel.sendMessage(InetAddress.getByName("127.0.0.1"), gameModel.getSelfPort(),
                    gameModel.createJoinMsg());

            Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/game.fxml"));

            Scene gameViewScene = new Scene(gameViewParent);

            Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

            window.setScene(gameViewScene);
            window.show();
        } catch (WrongArgException | InterruptedException ex) {
            alert.setContentText(ex.getMessage());
            alert.showAndWait();
        }
    }

    public void startButtonMoved() {
        startButton.setImage(movedStartButton);
    }

    public void startButtonExited() {
        startButton.setImage(exitedStartButton);
    }

    public void backButtonPressed(MouseEvent event) throws IOException {

        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/menu.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void backButtonMoved() {
        backButton.setImage(movedBackButton);
    }

    public void backButtonExited() {
        backButton.setImage(exitedBackButton);
    }
}
