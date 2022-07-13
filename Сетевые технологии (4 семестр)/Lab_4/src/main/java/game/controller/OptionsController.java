package game.controller;

import game.model.GameModel;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

public class OptionsController implements Initializable {
    private GameModel gameModel = GameModel.getInstance();

    private Image movedApplyButton = new Image("game/buttons/apply_button/apply_2.png");
    private Image exitedApplyButton = new Image("game/buttons/apply_button/apply_1.png");

    private Image movedBackButton = new Image("game/buttons/back_button/back_2.png");
    private Image exitedBackButton = new Image("game/buttons/back_button/back_1.png");

    @FXML
    private ImageView applyButton;

    @FXML
    private ImageView backButton;

    @FXML
    private TextField playerNameField;

    public OptionsController() throws IOException {
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        playerNameField.setText(gameModel.getMasterPlayerName());
    }

    public void applyButtonPressed(MouseEvent event) throws IOException {
        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/game/fxml/menu.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage)((Node)event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);

        gameModel.setMasterPlayerName(playerNameField.getText());
        window.show();
    }

    public void applyButtonMoved() {
        applyButton.setImage(movedApplyButton);
    }

    public void applyButtonExited() {
        applyButton.setImage(exitedApplyButton);
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
