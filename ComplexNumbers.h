#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <queue>
#include <string_view>
#include <cmath>

using namespace std;

class Complex {
public:
    Complex(double re = 0, double im = 0) {
        Re_ = re;
        Im_ = im;
    }

    double Re() const {
        return Re_;
    }

    double Im() const {
        return Im_;
    }

    Complex operator+ () const {
        return {Re_, Im_};
    }

    Complex operator- () const {
        return {-Re_, -Im_};
    }

    friend double Abs(Complex z);

private:
    double Re_;
    double Im_;
};

double Abs(const Complex z) {
    return sqrt(pow(z.Re(), 2) + pow(z.Im(), 2));
}

template<typename T>
Complex operator/ (const Complex u, const T& x) {
    Complex z(x);
    double ans_re = u.Re() * z.Re() + u.Im() * z.Im();
    double ans_im = -u.Re() * z.Im() + u.Im() * z.Re();
    return {ans_re / pow(Abs(z), 2), ans_im / pow(Abs(z), 2)};
}

template<typename T>
Complex operator* (const Complex& u, const T& x) {
    Complex z(x);
    double ans_re = u.Re() * z.Re() - u.Im() * z.Im();
    double ans_im = u.Re() * z.Im() + u.Im() * z.Re();
    return {ans_re, ans_im};
}

template<typename T>
Complex operator- (const Complex& u, const T& x) {
    Complex z(x);
    return {u.Re() - z.Re(), u.Im() - z.Im()};
}

template<typename T>
Complex operator+ (const Complex& u, const T& x) {
    Complex z(x);
    return {u.Re() + z.Re(), u.Im() + z.Im()};
}

template<typename T>
bool operator== (const Complex& z, const T& x) {
    Complex u(x);
    return u.Re() == z.Re() && u.Im() == z.Im();
}

template<typename T>
bool operator!= (const Complex& z, const T& x) {
    return !(z == x);
}
