#include "Complex.h"

Complex::Complex() : real(0), imag(0) {}

Complex::Complex(double r, double i) : real(r), imag(i) {}

Complex::Complex(const Complex& other) : real(other.real), imag(other.imag) {}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(const Complex& other) const {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
}

Complex Complex::operator/(const Complex& other) const {
    double denom = other.real * other.real + other.imag * other.imag;
    if (denom == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return Complex(
        (real * other.real + imag * other.imag) / denom,
        (imag * other.real - real * other.imag) / denom
    );
}

void Complex::print() const {
    cout << real;
    if (imag >= 0) cout << " + " << imag << "i";
    else cout << " - " << -imag << "i";
}
