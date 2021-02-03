package matrix;

import complex.Complex;

public class MatrixComplex {
    private final Complex[][] data;
    private final int N;  // number of rows
    private final int M;  // number of columns

    // Создает пустую матрицу размера n х m
    public MatrixComplex(int n, int m) {
        if (n < 0 || m < 0) {
            throw new RuntimeException("Negative matrix size");
        }

        this.N = n;
        this.M = m;
        data = new Complex[n][m];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                data[i][j] = new Complex(0, 0);
    }

    // Создает матрицу из двумерного массива
    public MatrixComplex(Complex[][] arr) {
        this(arr.length, arr[0].length);

        for (int i = 0; i < N; i++)
            data[i] = arr[i].clone();
    }

    // copy constructor
    public MatrixComplex(MatrixComplex other) {
        this(other.data);
    }

    public Complex get(int i, int j) {
        if (i > N || j > M || j < 0 || i < 0) {
            throw new IndexOutOfBoundsException("Matrix index out of bounds!");
        }

        return data[i][j];
    }

    // Умножение матриц
    public MatrixComplex mul(MatrixComplex other) {
        if (M != other.N) {
            throw new RuntimeException("Matrix size does not match!");
        }

        var temp = new MatrixComplex(N, other.M);

        for (int i = 0; i < this.N; i++)
            for (int j = 0; j < other.M; j++)
                for (int k = 0; k < this.M; k++)
                    temp.data[i][j] = temp.data[i][j].sum(this.data[i][k].mul(other.data[k][j]));

        return temp;
    }
    // Умножение матрицы на число
    public MatrixComplex mul(Complex num) {
        var temp = new MatrixComplex(this);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                temp.data[i][j] = temp.data[i][j].mul(num);

        return temp;
    }

    // Сложение матриц
    public MatrixComplex add(MatrixComplex other) {
        if (N != other.N || M != other.M) {
            throw new RuntimeException("Matrix size does not match!");
        }

        var temp = new MatrixComplex(N, M);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                temp.data[i][j] = this.data[i][j].sum(other.data[i][j]);

        return temp;
    }

    // Вычитание матриц
    public MatrixComplex sub(MatrixComplex other) {
        return this.add(other.mul(new Complex(-1.0, 0)));
    }

    // Транспонирование
    public MatrixComplex transpose() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        MatrixComplex temp = new MatrixComplex(N, M);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                temp.data[j][i] = this.data[i][j];

        return temp;
    }

    // Приведение матрицы к треугольной форме
    public MatrixComplex triangleForm() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        var triangle = new MatrixComplex(this);

        for (int k = 0; k < N; k++) {
            for (int i = k + 1; i < N; i++) {
                var K = triangle.data[i][k].div(triangle.data[k][k]);
                for (int j = 0; j < N; j++) {
                    triangle.data[i][j] = triangle.data[i][j].sub(triangle.data[k][j].mul(K));
                }
            }
        }

        return triangle;
    }

    // Вычисление детерминанта с помощью метода Гаусса (с помощью приведения к треугольной форме)
    public Complex det() {
        if (N != M) {
            throw new RuntimeException("Matrix must be square!");
        }

        var triangle = this.triangleForm();

        var det = new Complex(1, 0);
        for (int i = 0; i < N; i++)
            det = det.mul(triangle.data[i][i]);

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
