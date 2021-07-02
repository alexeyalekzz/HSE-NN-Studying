package sample.controllers;

import javafx.beans.binding.Bindings;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.paint.Paint;
import javafx.stage.Stage;
import org.javatuples.*;
import sample.Utils;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class TableRedactorController {
    public DatabaseController databaseController;
    private String tableName;
    private boolean editMode = false;
    private long editingID;

    @FXML private TableView<Tuple> tableView;
    @FXML private ToolBar toolBar;
    @FXML private TextField TableNameField;
    @FXML private Label ErrorLabel;
    @FXML private Button deleteTableButton;
    @FXML private Button addRowButton;
    @FXML private Button saveChangesButton;
    @FXML private Button clearTableButton;
    @FXML private Label psqlErrorLabel;

    static public Tuple createTuple(String[] arr) {
        return switch (arr.length) {
            case 1 -> new Unit<>(arr[0]);
            case 2 -> new Pair<>(arr[0], arr[1]);
            case 3 -> new Triplet<>(arr[0], arr[1], arr[2]);
            case 4 -> new Quartet<>(arr[0], arr[1], arr[2], arr[3]);
            case 5 -> new Quintet<>(arr[0], arr[1], arr[2], arr[3], arr[4]);
            case 6 -> new Sextet<>(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
            case 7 -> new Septet<>(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6]);
            case 8 -> new Octet<>(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7]);
            case 9 -> new Ennead<>(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8]);
            case 10 -> new Decade<>(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8], arr[9]);
            default -> throw new IllegalStateException("Unexpected value: " + arr.length);
        };
    }

    private void buildData() {
        try {
            ObservableList<Tuple> data = FXCollections.observableArrayList();
            ResultSet rs = databaseController.database.getTable(tableName);
            int colCount = rs.getMetaData().getColumnCount();

            while (rs.next()) {
                var arr = new String[colCount];
                for (int j = 0; j < colCount; j++) {
                    arr[j] = rs.getString(j + 1);
                }
                data.add(createTuple(arr));
            }
            tableView.setItems(data);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void init(DatabaseController db, String tableName) {
        databaseController = db;
        this.tableName = tableName;
        TableNameField.setText(tableName);

        try {
            ResultSet rs = databaseController.database.getTable(tableName);
            rs.next();

            int colCount = rs.getMetaData().getColumnCount();
            for (int i = 0; i < colCount; i++) {
                var colName = rs.getMetaData().getColumnName(i + 1);

                var column = new TableColumn<Tuple, String>(colName);
                    column.setPrefWidth(tableView.getPrefWidth() / colCount);
                    column.setCellValueFactory(new PropertyValueFactory<>("value" + i));

                var textField = new TextField();
                    textField.setPrefWidth((toolBar.getPrefWidth() / (colCount - 1)) - 10);

                if (!colName.contains("ИДЕНТИФИКАТОР")) {
                    toolBar.getItems().add(textField);
                }
                tableView.getColumns().add(column);
            }

            buildData();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void initialize() {
        tableView.setRowFactory((tv) -> {
            TableRow<Tuple> row = new TableRow<>();
            ContextMenu rowMenu = new ContextMenu();
            var edit = new MenuItem("Edit");
                edit.setOnAction(actionEvent -> {
                    var vals = row.getItem();
                    var textFields = toolBar.getItems();
                    for (int i = 1; i < vals.getSize(); i++) {
                        ((TextField) textFields.get(i - 1)).setText((String) vals.getValue(i));
                    }
                    psqlErrorLabel.setTextFill(Paint.valueOf("GREEN"));
                    psqlErrorLabel.setText("Edit mode #" + vals.getValue(0));
                    editMode = true;
                    editingID = Long.parseLong((String) vals.getValue(0));
                });

            var del = new MenuItem("Delete");
                del.setOnAction(actionEvent -> {
                    try {
                        databaseController.database.deleteRow(tableName, Long.parseLong((String) row.getItem().getValue(0)));
                    } catch (SQLException e) {
                        psqlErrorLabel.setText(e.getMessage());
                    }
                    buildData();
                });
                rowMenu.getItems().addAll(edit, del);

                row.contextMenuProperty().bind(
                        Bindings.when(row.emptyProperty())
                        .then((ContextMenu) null)
                        .otherwise(rowMenu));
                return row;
        });

        deleteTableButton.setOnAction(actionEvent -> {
            var stage = new Stage();
            Utils.createConfirmationWindow(
                stage,
                new Label("Вы действительно хотите удалить эту таблицу?\nЭто действие нельзя отменить"),
                cancelEvent -> stage.hide(),
                confirmEvent -> {
                    try {
                        databaseController.database.deleteTable(tableName);
                    } catch (SQLException e) {
                        psqlErrorLabel.setText(e.getMessage());
                    }
                    databaseController.refreshTables();
                    stage.hide();
                }
            );
        });

        clearTableButton.setOnAction(actionEvent -> {
            var stage = new Stage();
            Utils.createConfirmationWindow(
                stage,
                new Label("Вы действительно хотите очистить эту таблицу?\nЭто действие нельзя отменить"),
                cancelEvent -> stage.hide(),
                confirmEvent -> {
                    try {
                        databaseController.database.clearTable(tableName);
                    } catch (SQLException e) {
                        psqlErrorLabel.setText(e.getMessage());
                    }
                    buildData();
                    stage.hide();
                }
            );
        });

        addRowButton.setOnAction(actionEvent -> {
            if (!editMode) {
                String[] fields = new String[toolBar.getItems().size() + 1];
                fields[0] = null;

                int i = 1;
                for (Node node : toolBar.getItems()) {
                    TextField tf = ((TextField) node);
                    fields[i] = tf.getText();
                    tf.clear();
                    i++;
                }

                try {
                    databaseController.database.addTableRow(tableName, fields);
                } catch (SQLException e) {
                    psqlErrorLabel.setText(e.getMessage());
                }
                buildData();
            } else {
                psqlErrorLabel.setText("");
                psqlErrorLabel.setTextFill(Paint.valueOf("RED"));
                String[] fields = new String[toolBar.getItems().size() + 1];
                fields[0] = String.valueOf(editingID);

                int i = 1;
                for (Node node : toolBar.getItems()) {
                    TextField tf = ((TextField) node);
                    fields[i] = tf.getText();
                    tf.clear();
                    i++;
                }
                try {
                    databaseController.database.deleteRow(tableName, editingID);
                    databaseController.database.addTableRow(tableName, fields);
                } catch (SQLException e) {
                    psqlErrorLabel.setText(e.getMessage());
                }
                editMode = false;
            }
        });

        saveChangesButton.setOnAction(actionEvent -> {
            try {
                String newName = TableNameField.getText();

                if (!newName.equals(tableName)) {
                    var arrList = new ArrayList<String>();
                    for (var item : databaseController.database.getTableNames()) {
                        arrList.add(item.getText());
                    }
                    if (arrList.contains(newName)) {
                        ErrorLabel.setText("Таблица с таким именем уже существует!");
                        throw new SQLException();
                    }
                    databaseController.database.renameTable(tableName, newName);
                }
                if (newName.isEmpty()) {
                    ErrorLabel.setText("Введите название");
                    throw new SQLException();
                }
                ErrorLabel.getScene().getWindow().hide();
                databaseController.refreshTables();
            } catch (SQLException throwables) {
                throwables.printStackTrace();
            }
        });
    }
}