package program;

import javafx.application.Platform;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ThreadLocalRandom;


public class RequestsGenerator implements Runnable {
    private final BlockingQueue<Request> queue;
    private final int floorNum;
    private final GridPane reqPane;
    private int callNum;
    private final long lifetime;
    private final long startTime;

    RequestsGenerator(BlockingQueue<Request> queue, int floorNum, long lifetime, GridPane pane) {
        this.queue = queue;
        this.floorNum = floorNum;
        this.lifetime = lifetime;
        this.startTime = System.currentTimeMillis();
        this.reqPane = pane;
    }

    public void run() {
        while ((System.currentTimeMillis() - startTime) < lifetime) {
            try {
                Thread.sleep(ThreadLocalRandom.current().nextInt(0, 500 + 1));

                int from = ThreadLocalRandom.current().nextInt(0, floorNum);
                int to;
                do {
                    to = ThreadLocalRandom.current().nextInt(0, floorNum);
                } while (from == to);

                queue.put(new Request(from, to, callNum));

                Platform.runLater(() -> {
                    var l = ((Label) getNodeByRowIndex(floorNum - 1 - from));
                    l.setText(String.valueOf(Integer.parseInt(l.getText()) + 1));
                });

            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            callNum++;
        }
    }

    private Node getNodeByRowIndex(int row) {
        Node result = null;
        for (Node node : reqPane.getChildren()) {
            if(GridPane.getRowIndex(node) == row && GridPane.getColumnIndex(node) == 1) {
                result = node;
                break;
            }
        }
        return result;
    }
}
