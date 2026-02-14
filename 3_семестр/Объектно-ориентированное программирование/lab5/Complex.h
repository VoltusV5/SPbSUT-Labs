#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Complex {
private:
    T real;
    T imag;
public:
    Complex(T r = 0, T i = 0) : real(r), imag(i) {}

    Complex<T> operator+(const Complex<T>& other) const {
        return Complex<T>(real + other.real, imag + other.imag);
    }

    Complex<T> operator-(const Complex<T>& other) const {
        return Complex<T>(real - other.real, imag - other.imag);
    }

    Complex<T> operator*(const Complex<T>& other) const {
        return Complex<T>(real * other.real - imag * other.imag,
                          real * other.imag + imag * other.real);
    }

    Complex<T> operator/(const Complex<T>& other) const {
        T denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0) throw std::runtime_error("Division by zero in Complex");
        return Complex<T>((real * other.real + imag * other.imag) / denom,
                          (imag * other.real - real * other.imag) / denom);
    }

    void print() const {
        std::cout << "(" << real << ", " << imag << ")";
    }
};

#endif
