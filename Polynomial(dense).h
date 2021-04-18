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
#include <list>
#include <stdexcept>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

template<typename T>
class Polynomial {
public:
    Polynomial(const T& value = T(0)) {
        Polynomial_.reserve(1002);
        Polynomial_.push_back(value);
        Polynomial_.resize(this->Degree() + 1);
    }

    Polynomial(const std::vector<T>& polynomial) {
        Polynomial_.reserve(1002);
        Polynomial_ = polynomial;
        Polynomial_.resize(this->Degree() + 1);
    }

    template<typename It>
    Polynomial(It first, It last) {
        Polynomial_.reserve(1002);
        while (first != last) {
            Polynomial_.push_back(*first++);
        }
        Polynomial_.resize(this->Degree() + 1);
    }

    int Degree() const {
        int degree = Polynomial_.size() - 1;
        while (degree >= 0 && Polynomial_[degree] == T(0)) {
            --degree;
        }
        return degree;
    }

    T operator[] (int degree) const {
        if (degree <= this->Degree()) {
            return Polynomial_[degree];
        }
        return T(0);
    }

    T operator() (const T& x) const {
        T ans = T(0);
        for (int i = 0; i < this->Degree() + 1; ++i) {
            ans = ans * x + Polynomial_[this->Degree() - i];
        }
        return ans;
    }

    Polynomial& operator+= (const Polynomial& q) {
        int degree = std::max(this->Degree() + 1, q.Degree() + 1);
        Polynomial_.resize(degree);
        for (int i = 0; i < degree; ++i) {
            try {
                Polynomial_[i] += q[i];
            } catch (...) {
            }
        }
        Polynomial_.resize(this->Degree() + 1);
        return *this;
    }

    Polynomial& operator-= (const Polynomial& q) {
        int degree = std::max(this->Degree() + 1, q.Degree() + 1);
        Polynomial_.resize(degree);
        for (int i = 0; i < degree; ++i) {
            try {
                Polynomial_[i] -= q[i];
            } catch (...) {
            }
        }
        Polynomial_.resize(this->Degree() + 1);
        return *this;
    }

    Polynomial& operator*= (const Polynomial& x) {
        int degree = this->Degree() + x.Degree() + 2;
        Polynomial_.resize(degree);
        Polynomial p(this->Polynomial_);
        Polynomial q(x.Polynomial_);
        for (int i = 0; i < degree; ++i) {
            T monom = T(0);
            for (int j = 0; j < i + 1; ++j) {
                try {
                    monom += q[j] * p[i - j];
                } catch (...) {
                }
            }
            Polynomial_[i] = monom;
        }
        Polynomial_.resize(this->Degree() + 1);
        return *this;
    }

    auto begin() const {
        return Polynomial_.begin();
    }

    auto end() const {
        return Polynomial_.end();
    }

    friend std::ostream& operator<< (std::ostream& out, const Polynomial& polynomial) {
        int degree = polynomial.Degree();
        if (degree == -1) {
            out << T(0);
            return out;
        }
        if (degree == 0) {
            out << polynomial[0];
            return out;
        }
        if (degree == 1) {
            first_coefficient(polynomial, degree, out);
            out << "x";
        } else {
            first_coefficient(polynomial, degree, out);
            out << "x^" << degree;
        }
        for (int i = degree - 1; i > 0; --i) {
            if (polynomial[i] == T(0)) {
                continue;
            }
            if (i == 1) {
                coefficient(polynomial, i, out);
                out << "x";
            } else {
                coefficient(polynomial, i, out);
                out << "x^" << i;
            }
        }
        if (polynomial[0] < T(0)) {
            out << polynomial[0];
        } else if (polynomial[0] > T(0)) {
            out << "+" << polynomial[0];
        }
        return out;
    }

    friend bool operator== (const Polynomial& p, const Polynomial& q) {
        return p.Polynomial_ == q.Polynomial_;
    }

    friend bool operator!= (const Polynomial& p, const Polynomial& q) {
        return p.Polynomial_ != q.Polynomial_;
    }

    friend Polynomial operator+ (const Polynomial& p, const Polynomial& q) {
        Polynomial ans = p;
        ans += q;
        return ans;
    }

    friend Polynomial operator- (const Polynomial& p, const Polynomial& q) {
        Polynomial ans = p;
        ans -= q;
        return ans;
    }

    friend Polynomial operator* (const Polynomial& p, const Polynomial& q) {
        Polynomial ans = p;
        ans *= q;
        return ans;
    }

    friend Polynomial operator& (const Polynomial& p, const Polynomial& q) {
        Polynomial x(T(1));
        Polynomial ans;
        for (int i = 0; i < p.Degree() + 1; ++i) {
            ans += x * p[i];
            x *= q;
        }
        return ans;
    }

    friend Polynomial operator/ (const Polynomial& p, const Polynomial& q) {
        if (p.Degree() < q.Degree()) {
            return {};
        }
        Polynomial temp = p;
        Polynomial result;
        Polynomial x = Polynomial(std::vector{T(0), T(1)});
        while (temp.Degree() >= q.Degree()) {
            Polynomial coeff(temp[temp.Degree()] / q[q.Degree()]);
            while (coeff.Degree() < temp.Degree() - q.Degree()) {
                coeff *= x;
            }
            temp -= q * coeff;
            result += coeff;
        }
        return result;
    }

    friend Polynomial operator% (const Polynomial& p, const Polynomial& q) {
        return p - (p / q) * q;
    }

    friend Polynomial operator, (Polynomial p, Polynomial q) {
        if (p == T(0) && q == T(0)) {
            return T(0);
        }
        while (q != T(0)) {
            p = p % q;
            auto r = q;
            q = p;
            p = r;
        }
        p = p / p[p.Degree()];
        return p;
    }

private:
    std::vector<T> Polynomial_;
};

template<typename N>
void first_coefficient(const Polynomial<N>& polynomial, int degree, std::ostream& out) {
    if (polynomial[degree] == N(-1)) {
        out << "-";
    } else if (polynomial[degree] == N(1)) {
    } else {
        out << polynomial[degree] << "*";
    }
}

template<typename N>
void coefficient(const Polynomial<N>& polynomial, int degree, std::ostream& out) {
    if (polynomial[degree] < N(0)) {
        first_coefficient(polynomial, degree, out);
    } else if (polynomial[degree] > N(0)) {
        out << "+";
        first_coefficient(polynomial, degree, out);
    }
}
