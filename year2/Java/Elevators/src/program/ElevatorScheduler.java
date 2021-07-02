package program;

import javafx.application.Platform;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;

import java.util.concurrent.BlockingQueue;

public class ElevatorScheduler implements Runnable {
    private final BlockingQueue<Request> queue;
    private int iter;
    private final long lifetime;
    private final long startTime;
    private final Elevator[] elevators;
    private final int floorsN;
    private GridPane elevatorsPane;

    public ElevatorScheduler(BlockingQueue<Request> queue, int elevatorsN, int floorsN, int lifetime, GridPane elevatorsPane) {
        this.queue = queue;
        this.lifetime = lifetime;
        this.floorsN = floorsN;
        this.startTime = System.currentTimeMillis();
        this.elevators = new Elevator[elevatorsN];
        for (int i = 0; i < elevators.length; i++) {
            elevators[i] = new Elevator(i);
        }

        this.elevatorsPane = elevatorsPane;
    }

    private int chooseElevator(Elevator[] elevators, Request request) {
        int cheduleTo = 0;
        int score = -1000;
        int callDirection = request.getFrom() - request.getTo();
        for (int i = 0; i < elevators.length; i++) {
            int elevatorDirection = elevators[i].getCurrent() - elevators[i].getDest();
            boolean isSameDirection = true;
            if ((callDirection * elevatorDirection) < 0) {
                isSameDirection = false;
            }

            int currentScore = 0;

            if (isSameDirection) {
                if (request.getFrom() == elevators[i].getCurrent()) {
                    currentScore += 500;
                }
                currentScore -= elevators[i].getCallsNum();
                if (elevatorDirection < 0) {
                    if (request.getFrom() < elevators[i].getCurrent()) {
                        currentScore -= 500;
                    }
                } else {
                    if (request.getFrom() > elevators[i].getCurrent()) {
                        currentScore -= 500;
                    }
                }

                currentScore -= Math.abs(request.getFrom() - elevators[i].getCurrent());
                currentScore += Math.abs(elevators[i].getCurrent() - elevators[i].getDest());
            }
            else if (!isSameDirection) {
                currentScore -= elevators[i].getCallsNum();
                currentScore -= Math.abs(elevators[i].getCurrent() - elevators[i].getDest());
                currentScore -= Math.abs(request.getFrom() - elevators[i].getDest());
            } else {
                currentScore -= Math.abs(request.getFrom() - elevators[i].getCurrent());
            }

            if (currentScore > score) {
                score = currentScore;
                cheduleTo = i;
            }
        }
        return cheduleTo;
    }

    public void run() {
        int stillProcessing = 0;
        while (((System.currentTimeMillis() - startTime) < lifetime) || (stillProcessing > 0)) {
            stillProcessing = 0;
            try {
                Thread.sleep(500);
            } catch (InterruptedException ignored) {
            }

            System.out.println("\nIteration " + iter + "\n");
            iter += 1;

            while (queue.peek() != null) {
                try {
                    Request request = queue.take();
                    elevators[chooseElevator(elevators, request)].addCall(request);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }

            for (int i = 0; i < elevators.length; i++) {
                int step = elevators[i].move();
                stillProcessing += elevators[i].getCallsNum();

                if (step != 0) {
                    System.out.println("Moving elevator #" + i);
                    moveOnPane(i, step);
                }
            }
        }
    }

    private void moveOnPane(int i, int step) {
        Platform.runLater(() -> {
            int curr = elevators[i].getCurrent();
            System.out.println("Current: " + curr);
            System.out.println("Prev: " + (curr - step));
            System.out.println("Current on pane: " + (floorsN - curr - 1));
            System.out.println("Prev on pane: " + (floorsN - (curr - step) - 1));
            System.out.println("Step: " + step);

            elevatorsPane.getChildren().remove(
                    getNodeByRowColumnIndex(floorsN - (curr - step) - 1, i)
            );

            elevatorsPane.add(Elevator.draw(), i, floorsN - curr - 1);
        });
    }

    private Node getNodeByRowColumnIndex(int row, int column) {
        Node result = null;
        for (Node node : elevatorsPane.getChildren()) {
            if(GridPane.getRowIndex(node) == row && GridPane.getColumnIndex(node) == column) {
                result = node;
                break;
            }
        }
        return result;
    }
}
