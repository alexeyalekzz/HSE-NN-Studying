package program;

import javafx.fxml.FXML;
import javafx.geometry.VPos;
import javafx.scene.control.Label;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.RowConstraints;
import javafx.scene.text.Font;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class MainWindowController {
    @FXML private GridPane elevatorsPane;
    @FXML private GridPane requestsPane;

    void initialize(int floorsN, int elevatorsN) {
        elevatorsPane.getRowConstraints().remove(0);
        requestsPane.getRowConstraints().remove(0);

        for (int i = 0; i < floorsN; i++) {
            elevatorsPane.getRowConstraints().add(new RowConstraints(Elevator.HEIGHT));
            var rc = new RowConstraints(Elevator.HEIGHT);
                rc.setValignment(VPos.CENTER);
                rc.setPrefHeight(1080.0 / floorsN);
            var l1 = new Label("Запросы на этаже №" + (floorsN - i) + ": ");
                l1.setFont(Font.font(19));
            var l2 = new Label("0");
                l2.setFont(Font.font(19));
            requestsPane.add(l1, 0, i);
            requestsPane.add(l2, 1, i);
            requestsPane.getRowConstraints().add(rc);
        }
        for (int i = 0; i < elevatorsN; i++) {
            elevatorsPane.getColumnConstraints().add(new ColumnConstraints(Elevator.WIDTH));
            elevatorsPane.add(Elevator.draw(), i, floorsN - 1);
        }

        int delay = 10000;
        BlockingQueue<Request> queue = new ArrayBlockingQueue<>(50);
        RequestsGenerator sender = new RequestsGenerator(queue, floorsN, delay * 500, requestsPane);
        ElevatorScheduler receiver = new ElevatorScheduler(queue, elevatorsN, floorsN,delay * 500, elevatorsPane);

        new Thread(sender).start();
        new Thread(receiver).start();
    }
}
