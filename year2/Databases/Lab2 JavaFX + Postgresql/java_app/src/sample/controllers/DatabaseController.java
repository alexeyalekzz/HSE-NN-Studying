package sample.controllers;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import org.javatuples.Tuple;
import sample.Database;
import sample.Utils;

import static sample.controllers.TableRedactorController.createTuple;

public class DatabaseController {
    public Database database;

    @FXML private MenuBar DatabaseMenuBar;
    @FXML private Menu DBSettings;
    @FXML private Menu tableRedactor;
    @FXML private ScrollPane scrollPane;
    @FXML private Label psqlErrorLabel;
    @FXML private Button deleteDatabaseButton;

    private void openTableRedactor(String tableName) {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample/assets/TableRedactor.fxml"));

        try {
            loader.load();
        } catch (IOException e) {
            e.printStackTrace();
        }

        var controller = ((TableRedactorController) loader.getController());
            controller.init(this, tableName);

        var stage = new Stage();
            stage.setScene(new Scene(loader.getRoot()));
            stage.setTitle("Редактор таблицы " + tableName);
            stage.show();
            stage.setOnCloseRequest(windowEvent -> refreshTables());
    }

    private TableView<Tuple> createFromTable(String tableName, double width, double height) {
        var titleCol = new TableColumn<Tuple, String>(tableName);
            titleCol.setPrefWidth(width);

        var tv = new TableView<Tuple>();
            tv.setPrefWidth(width);
            tv.setPrefHeight(height);
            tv.getColumns().add(titleCol);

        try {
            database.connect();
            ResultSet rs = database.getTable(tableName);
            boolean isNotEmpty = rs.next();

            int colCount = rs.getMetaData().getColumnCount();
            for (int i = 0; i < colCount; i++) {
                var colName = rs.getMetaData().getColumnName(i + 1);

                var column = new TableColumn<Tuple, String>(colName);
                    column.setPrefWidth(width / colCount);
                    column.setCellValueFactory(new PropertyValueFactory<>("value" + i));

                titleCol.getColumns().add(column);
            }

            ObservableList<Tuple> data = FXCollections.observableArrayList();
            if (isNotEmpty) {
                do {
                    var arr = new String[colCount];
                    for (int j = 0; j < colCount; j++) {
                        arr[j] = rs.getString(j + 1);
                    }
                    data.add(createTuple(arr));
                } while(rs.next());
            }
            tv.setItems(data);

        } catch (SQLException e) {
            e.printStackTrace();
        }

        return tv;
    }

    public void refreshTables() {
        try {
            var pane = new GridPane();
            pane.setStyle("-fx-background-color: #FFE4B5");
            pane.setPrefWidth(scrollPane.getPrefWidth());
            pane.setPrefHeight(4000);
            pane.setHgap(8);
            pane.setVgap(8);

            var arr = new ArrayList<TableView<Tuple>>();
            for (var name : database.getTableNames()) {
                double width = scrollPane.getPrefWidth() / 2 - pane.getVgap() * 2;
                double height = scrollPane.getPrefHeight() / 2 - pane.getHgap() * 2;

                var edit = new MenuItem("Open in table redactor");
                edit.setOnAction(actionEvent -> openTableRedactor(name.getText()));

                var del = new MenuItem("Delete");
                del.setOnAction(actionEvent -> {
                    var stage = new Stage();
                    Utils.createConfirmationWindow(
                        stage,
                        new Label("Вы действительно хотите удалить эту таблицу?\nЭто действие нельзя отменить"),
                        cancelEvent -> stage.hide(),
                        confirmEvent -> {
                            try {
                                database.deleteTable(name.getText());
                            } catch (SQLException e) {
                                psqlErrorLabel.setText(e.getMessage());
                            }
                            refreshTables();
                            stage.hide();
                        }
                    );
                });

                var menu = new ContextMenu();
                    menu.getItems().addAll(edit, del);

                var table = createFromTable(name.getText(), width, height);
                    table.setContextMenu(menu);

                arr.add(table);
            }

            for (int i = 0; i < arr.size(); i++) {
                pane.add(arr.get(i), i % 2, i / 2);
            }

            ObservableList<MenuItem> correct = FXCollections.observableArrayList();
            var names = database.getTableNames();
            for (var name : names) {
                if (!name.getText().toLowerCase().contains("temp")) {
                    correct.add(name);
                }
            }
            tableRedactor.getItems().setAll(correct);
            scrollPane.setContent(pane);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void init(Database database) {
        this.database = database;
        try {
            database.connect();
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }

        refreshTables();

        for(var item : tableRedactor.getItems()) {
            item.setOnAction(actionEvent -> openTableRedactor(item.getText()));
        }
    }

    @FXML
    void initialize() {
        // delete database
        DBSettings.getItems().get(0).setOnAction(actionEvent -> {
            var stage = new Stage();
            Utils.createConfirmationWindow(
                stage,
                new Label("Вы действительно хотите удалить эту базу данных?\nЭто действие нельзя отменить"),
                cancelEvent -> stage.hide(),
                confirmEvent -> {
                    String dbName = database.getName();
                    database.getServer().deleteDB(dbName);
                    stage.hide();
                    psqlErrorLabel.getScene().getWindow().hide();
                }
            );
        });
        deleteDatabaseButton.setOnAction(DBSettings.getItems().get(0).getOnAction());
        deleteDatabaseButton.setText("Удалить\nбазу данных");

        // clear all tables
        DatabaseMenuBar.getMenus().get(1).getItems().get(2).setOnAction(actionEvent -> {
            var stage = new Stage();
            Utils.createConfirmationWindow(
                stage,
                new Label("Вы действительно хотите очистить все таблицы?\nЭто действие нельзя отменить"),
                cancelEvent -> stage.hide(),
                confirmEvent -> {
                    try {
                        database.clearAllTables();
                    } catch (SQLException throwables) {
                        throwables.printStackTrace();
                    }
                    stage.hide();
                    refreshTables();
                }
            );
        });
    }
}
