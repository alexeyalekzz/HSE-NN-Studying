package matrix;

public class MatrixDouble {
    private final double[][] data;
    private final int N;  // number of rows
    private final int M;  // number of columns

    // Создает пустую матрицу размера n х m
    public MatrixDouble(int n, int m) {
        if (n < 0 || m < 0) {
            throw new RuntimeException("Negative matrix size");
        }

        this.N = n;
        this.M = m;
        data = new double[n][m];
    }

    // Создает матрицу из двумерного массива
    public MatrixDouble(double[][] arr) {
        this(arr.length, arr[0].length);

        for (int i = 0; i < N; i++)
            data[i] = arr[i].clone();
    }

    // copy constructor
    public MatrixDouble(MatrixDouble other) {
        this(other.data);
    }

    public double get(int i, int j) {
        if (i > N || j > M || j < 0 || i < 0) {
            throw new IndexOutOfBoundsException("Matrix index out of bounds!");
        }

        return data[i][j];
    }

    // Умножение матриц
    public MatrixDouble mul(MatrixDouble other) {
        if (M != other.N) {
            throw new RuntimeException("Matrix size does not match!");
        }

        var temp = new MatrixDouble(N, other.M);

        for (int i = 0; i < this.N; i++)
            for (int j = 0; j < other.M; j++)
                for (int k = 0; k < this.M; k++)
                    temp.data[i][j] += this.data[i][k] * other.data[k][j];

        return temp;
    }
    // Умножение матрицы на число
    public MatrixDouble mul(double num) {
        var temp = new MatrixDouble(this);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                temp.data[i][j] *= num;

        return temp;
    }

    // Сложение матриц
    public MatrixDouble add(MatrixDouble other) {
        if (N != other.N || M != other.M) {
            throw new RuntimeException("Matrix size does not match!");
        }

        var temp = new MatrixDouble(N, M);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                temp.data[i][j] = this.data[i][j] + other.data[i][j];

        return temp;
    }

    // Вычитание матриц
    public MatrixDouble sub(MatrixDouble other) {
        return this.add(other.mul(-1.0));
    }

    // Транспонирование
    public MatrixDouble transpose() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        MatrixDouble temp = new MatrixDouble(N, M);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                temp.data[j][i] = this.data[i][j];

        return temp;
    }

    // Приведение матрицы к треугольной форме
    public MatrixDouble triangleForm() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        var triangle = new MatrixDouble(this);

        for (int k = 0; k < N; k++) {
            for (int i = k + 1; i < N; i++) {
                var K = triangle.data[i][k] / triangle.data[k][k];
                for (int j = 0; j < N; j++) {
                    triangle.data[i][j] -= triangle.data[k][j] * K;
                }
            }
        }

        return triangle;
    }

    // Вычисление детерминанта с помощью метода Гаусса (с помощью приведения к треугольной форме)
    public double det() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        var triangle = this.triangleForm();

        double det = 1;
        for (int i = 0; i < N; i++)
            det *= triangle.data[i][i];

        return det;
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();

        for (var row : data) {
            for (var n : row) {
                s.append(n).append(' ');
            }
            s.append('\n');
        }

        return s.toString();
    }
}
