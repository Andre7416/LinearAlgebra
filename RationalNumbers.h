#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <queue>
#include <string_view>
#include <cmath>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1) {
        int del = gcd(numerator, denominator);
        del = denominator > 0 ? del : -del;
        Numerator_ = numerator / del;
        Denominator_ = denominator / del;
    }

    int Numerator() const {
        return Numerator_;
    }

    int Denominator() const {
        return Denominator_;
    }

    Rational operator+ () const {
        return {Numerator_, Denominator_};
    }

    Rational operator- () const {
        return {-Numerator_, Denominator_};
    }

    template<typename T>
    Rational& operator+= (T x) {
        *this = *this + x;
        return *this;
    }

    template<typename T>
    Rational& operator-= (T x) {
        *this = *this - x;
        return *this;
    }

    template<typename T>
    Rational& operator*= (T x) {
        *this = *this * x;
        return *this;
    }

    template<typename T>
    Rational& operator/= (T x) {
        *this = *this / x;
        return *this;
    }

    Rational& operator-- () {
        this->Numerator_ -= this->Denominator_;
        return *this;
    }

    Rational& operator++ () {
        this->Numerator_ += this->Denominator_;
        return *this;
    }

    Rational operator-- (int) {
        Rational p(*this);
        this->Numerator_ -= this->Denominator_;
        return p;
    }

    Rational operator++ (int) {
        Rational p(*this);
        this->Numerator_ += this->Denominator_;
        return p;
    }

private:
    int Numerator_;
    int Denominator_;
};

template<typename T>
Rational operator/ (const Rational& p, const T& x) {
    Rational q(x);
    int ans_numerator = p.Numerator() * q.Denominator();
    int ans_denominator = p.Denominator() * q.Numerator();
    return {ans_numerator, ans_denominator};
}

template<typename T>
Rational operator* (const Rational& p, const T& x) {
    Rational q(x);
    int ans_numerator = p.Numerator() * q.Numerator();
    int ans_denominator = p.Denominator() * q.Denominator();
    return {ans_numerator, ans_denominator};
}

template<typename T>
Rational operator- (const Rational& p, const T& x) {
    Rational q(x);
    int ans_numerator = p.Numerator() * q.Denominator() - q.Numerator() * p.Denominator();
    int ans_denominator = p.Denominator() * q.Denominator();
    return {ans_numerator, ans_denominator};
}

template<typename T>
Rational operator+ (const Rational& p, const T& x) {
    Rational q(x);
    int ans_numerator = p.Numerator() * q.Denominator() + q.Numerator() * p.Denominator();
    int ans_denominator = p.Denominator() * q.Denominator();
    return {ans_numerator, ans_denominator};
}

template<typename T>
bool operator== (const Rational& p, const T& x) {
    Rational q(x);
    return p.Numerator() == q.Numerator() && p.Denominator() == q.Denominator();
}

template<typename T>
bool operator!= (const Rational& p, const T& x) {
    return !(p == x);
}
