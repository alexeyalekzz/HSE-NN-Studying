package program;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("assets/StartWindow.fxml"));
        var stage = new Stage();
            stage.setTitle("Старт");
            stage.setScene(new Scene(loader.load()));
            stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
