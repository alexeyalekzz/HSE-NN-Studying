package program;

import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;
import java.util.ArrayList;


public class Elevator {
    static final int WIDTH = 70;
    static final int HEIGHT = 110;
    private int currentFloor;
    private int destination;
    private final int num;
    private int callsNum;
    ArrayList<Request> schedule;

    static public Pane draw() {
        var line = new Line();
        line.setStartX(WIDTH / 2.0);
        line.setEndX(WIDTH / 2.0);
        line.setStartY(0);
        line.setEndY(HEIGHT);

        var pane = new Pane(line);
        pane.setStyle("-fx-background-color: GREY");

        return pane;
    }

    Elevator(int num) {
        this.num = num;
        this.schedule = new ArrayList<>();
    }

    public int getCurrent() {
        return currentFloor;
    }

    public int getDest() {
        return destination;
    }

    public int getCallsNum() {
        return callsNum;
    }

    public void addCall(Request request) {
        System.out.println("Call " + request.getNum() + " scheduled to elevator " + num);


        // В зависимости от направления движения лифта, изменяем точку назначения
        if (currentFloor < destination) {
            if (request.getTo() > destination) {
                destination = request.getTo();
            }
        } else if (currentFloor > destination) {
            if (request.getTo() < destination) {
                destination = request.getTo();
            }
        } else {
            int diff = 0;
            for (Request value : schedule) {
                if (Math.abs(currentFloor - value.getTo()) > diff) {
                    diff = Math.abs(currentFloor - value.getTo());
                    destination = value.getTo();
                }
            }
        }
        schedule.add(request);
    }

    public int move() {
        for (int i = 0; i < schedule.size(); i++) {
            if ((schedule.get(i).getFrom() == currentFloor) && !schedule.get(i).isTaken()) {
                callsNum += 1;
                schedule.set(i, schedule.get(i).take());
                System.out.println("Request " + schedule.get(i).getNum() +
                            " passenger taken on floor " + schedule.get(i).getFrom() +
                            " by elevator " + num);
            }
            if ((schedule.get(i).getTo() == currentFloor) && (schedule.get(i).isTaken())) {
                callsNum -= 1;
                System.out.println("Request " + schedule.get(i).getNum() +
                            " passenger delivered to floor " + schedule.get(i).getTo() +
                            " by elevator " + num);
                schedule.remove(i);
            }
        }

        // Двигаем лифт
        if (currentFloor < destination) {
            currentFloor += 1;
            return +1;
        } else if (currentFloor > destination) {
            currentFloor -= 1;
            return -1;
        } else {
            int diff = 0;
            for (Request request : schedule) {
                if (Math.abs(currentFloor - request.getTo()) > diff) {
                    diff = Math.abs(currentFloor - request.getTo());
                    destination = request.getTo();
                }
            }
            return 0;
        }
    }
}
