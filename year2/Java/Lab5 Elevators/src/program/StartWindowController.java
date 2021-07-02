package program;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;

public class StartWindowController {
    @FXML private TextField floorsNumber;
    @FXML private TextField elevatorsNumber;
    @FXML private Button startButton;
    @FXML private Label errorLabel;

    @FXML
    void initialize() {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("assets/MainWindow.fxml"));
        startButton.setOnAction(actionEvent -> {
            try {
                int e = Integer.parseInt(elevatorsNumber.getText());
                int f = Integer.parseInt(floorsNumber.getText());

                if (e < 0 || f < 0) {
                    throw new NumberFormatException();
                }

                Stage stage = new Stage();
                      stage.setScene(new Scene(loader.load()));

                ((MainWindowController) loader.getController()).initialize(f, e);
                stage.show();
                errorLabel.getScene().getWindow().hide();
                stage.setOnCloseRequest(windowEvent -> System.exit(0));
            } catch (NumberFormatException ex) {
                errorLabel.setText("Вы должны указать целое\n положительноe число");
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }
}
