package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.postgresql.PGProperty;
import sample.controllers.MainWindowController;

import java.sql.*;
import java.util.Properties;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample/assets/MainWindow.fxml"));
        loader.load();

        Server postgresql = new Server("localhost", "5432", "java_app", "1234");

        ((MainWindowController)loader.getController()).setServer(postgresql);

        primaryStage.setTitle("Start screen");
        primaryStage.setScene(new Scene(loader.getRoot(), 1280, 800));
        primaryStage.show();
    }


    public static void main(String[] args) {
       launch(args);
    }
}
