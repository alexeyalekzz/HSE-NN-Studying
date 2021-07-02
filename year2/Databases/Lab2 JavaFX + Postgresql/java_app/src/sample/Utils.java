package sample;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class Utils {
    static public Pane createConfirmationWindow(Stage stage, Label text, EventHandler<ActionEvent> cancelAction, EventHandler<ActionEvent> confirmAction) {
        StackPane.setAlignment(text, Pos.CENTER);

        Button confirm = new Button("Да");
               confirm.setPrefSize(100, 50);
               confirm.setOnAction(confirmAction);

        Button cancel = new Button("Отмена");
               cancel.setPrefSize(100, 50);
               cancel.setOnAction(cancelAction);

        StackPane.setAlignment(cancel, Pos.BOTTOM_RIGHT);
        StackPane.setMargin(cancel, new Insets(15));
        StackPane.setAlignment(confirm, Pos.BOTTOM_LEFT);
        StackPane.setMargin(confirm, new Insets(15));

        var pane = new StackPane(text, confirm, cancel);
            pane.setStyle("-fx-background-color: #FFE4B5");

        stage.setScene(new Scene(pane, 400, 200));
        stage.setTitle("Подтверждение");
        stage.show();

        return pane;
    }
}
