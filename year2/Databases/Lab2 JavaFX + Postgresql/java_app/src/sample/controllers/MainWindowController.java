package sample.controllers;


import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.stage.Stage;
import sample.Database;
import sample.Server;

import java.sql.SQLException;

public class MainWindowController {
    private Server server;

    @FXML private ComboBox<String> DBsDropList;
    @FXML private Button ConnectToDBButton;
    @FXML private Button CreateNewDBButton;
    @FXML private Label ErrorLabel;

    @FXML
    void initialize() {
        DBsDropList.setOnMouseClicked(mouseEvent -> {
            DBsDropList.setItems(server.listDownAllDatabases());
        });

        ConnectToDBButton.setOnAction(actionEvent -> {
            if (!DBsDropList.getSelectionModel().isEmpty()) {
                String dbName = DBsDropList.getSelectionModel().getSelectedItem();
                FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample/assets/Database.fxml"));

                Database db = null;
                try {
                    db = server.getDatabase(dbName);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                if (db != null && db.getConnection() == null) {
                    try {
                        loader.load();
                        ((DatabaseController) loader.getController()).init(db);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    Stage stage = new Stage();
                    stage.setScene(new Scene(loader.getRoot()));
                    stage.setTitle(dbName);
                    Database finalDb = db;
                    stage.setOnCloseRequest(windowEvent -> {
                        try {
                            finalDb.disconnect();
                        } catch (SQLException throwables) {
                            throwables.printStackTrace();
                        }
                    });
                    stage.show();
                } else {
                    ErrorLabel.setText("Вы уже подключены к этой базе данных!");
                }
                DBsDropList.getSelectionModel().clearSelection();
            } else {
                ErrorLabel.setText("Укажите базу данных или создайте новую");
            }
        });

        CreateNewDBButton.setOnAction(actionEvent -> {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample/assets/CreateDB.fxml"));

            try {
                loader.load();
                ((CreateDBController) loader.getController()).setServer(server);
            } catch (Exception e) {
                e.printStackTrace();
            }

            Stage stage = new Stage();
            stage.setScene(new Scene(loader.getRoot()));
            stage.setTitle("Создать базу данных");
            stage.show();
        });
    }

    public void setServer(Server server) {
        this.server = server;
    }
}