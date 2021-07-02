package program;

public class Request {
    private final int from, to, num;
    private boolean taken;

    Request(int from, int to, int num) {
        this.from = from;
        this.to = to;
        this.num = num;
        this.taken = false;
    }

    public Request take() {
        taken = true;
        return this;
    }

    public int getFrom() { return from; }
    public int getTo() { return to; }
    public int getNum() { return num; }
    public boolean isTaken() { return taken; }
}
