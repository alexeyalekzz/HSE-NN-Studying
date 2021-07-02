package sample.controllers;

import java.io.IOException;
import java.sql.*;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import sample.Database;
import sample.Server;

public class CreateDBController {
    private Server server;

    @FXML private TextField newDBNameField;
    @FXML private Button CreateDBButton;
    @FXML private Label errorLable;

    @FXML
    void initialize() {
        // Change window on button pressed
        CreateDBButton.setOnAction(actionEvent -> {
            String newDBname = newDBNameField.getText();
            boolean contains = server.listDownAllDatabases().contains(newDBname);
            if (!newDBname.isEmpty() && !contains) {
                CreateDBButton.getScene().getWindow().hide();
                FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample/assets/Database.fxml"));

                Database d = null;
                try {
                    loader.load();
                    d = server.createDB(newDBname);
                } catch (IOException | SQLException e) {
                    e.printStackTrace();
                }

                ((DatabaseController) loader.getController()).init(d);
                Stage stage = new Stage();
                stage.setScene(new Scene(loader.getRoot()));
                stage.setTitle(newDBname);
                stage.show();
            } else if (newDBname.isEmpty()) {
                errorLable.setText("Поле пустое!");
            } else {
                errorLable.setText("База данных с таким именем уже существует");
            }
        });
    }

    public void setServer(Server server) {
        this.server = server;
    }
}


