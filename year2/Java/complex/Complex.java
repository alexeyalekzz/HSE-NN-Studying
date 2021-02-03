package complex;

public class Complex {
    private final double real, imag;

    public Complex(double r, double i) {
        real = r;
        imag = i;
    }
    public Complex(double r) {
        this(r, 0);
    }
    public Complex() {
        this(0, 0);
    }

    // Copy constructor
    public Complex(Complex other) {
        real = other.real;
        imag = other.imag;
    }

    // Возвращает действительную часть
    public double real() {
        return real;
    }

    // Возвращает мнимую часть
    public double imag() {
        return imag;
    }

    // Сложение комплексных чисел
    public Complex sum(Complex other) {
        return new Complex(real + other.real, imag + other.imag);
    }

    // Вычитание комплексных чисел
    public Complex sub(Complex other) {
        return new Complex(real - other.real, imag - other.imag);
    }

    // Умножение комплексных чисел
    public Complex mul(Complex other) {
        var r = real * other.real - imag * other.imag;
        var i = real * other.imag + imag * other.real;

        return new Complex(r, i);
    }

    // Деление комплексных чисел
    public Complex div(Complex other) {
        var den = other.real * other.real + other.imag * other.imag;
        var r = (real * other.real + imag * other.imag) / den;
        var i = (imag * other.real + real * other.imag) / den;

        return new Complex(r, i);
    }

    // Возвращает комплексно-сопряжённое число
    public Complex conj() {
        return new Complex(real, -imag);
    }

    // Возвращает модуль комплексного числа
    public double abs() {
        return Math.sqrt(real * real + imag * imag);
    }

    // Возвращает строковое представление числа в экспоненциальной форме: |n| * e^(Ф * i)
    public String getExpForm() {
        return String.format("%.2f * e^%.2fi", this.abs(), Math.acos(real / this.abs()));
    }

    // Возвращает строковое представление числа в алгебраической форме: n.real + n.imag * i
    public String getAlgForm() {
        return String.format("%.2f " + (imag == 0 ? "" : (imag < 0 ? "- " : "+ ") + "%.2f"), real, imag < 0 ? imag * -1 : imag);
    }

    // Возвращает строковое представление числа в тригонометрической форме: |n| * (cosФ + sinФ * i)
    public String getTrigonomForm() {
        var r = this.abs();
        if (r > 0) {
            var cos = real / r;
            var sin = imag / r;
            return String.format("%.2f * (%.2f " + (sin < 0 ? "+ " : "- " + "%.2fi)"), r, cos, sin < 0 ? sin * -1 : sin);
        } else {
            return "0";
        }
    }

    @Override
    public String toString() {
        return getAlgForm();
    }
}
