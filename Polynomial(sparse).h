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
        Polynomial_[0] = value;
        this->Normalize();
    }

    Polynomial(const std::vector<T>& polynomial) {
        for (size_t i = 0; i < polynomial.size(); ++i) {
            Polynomial_[i] = polynomial[i];
        }
        this->Normalize();
    }

    template<typename It>
    Polynomial(It first, It last) {
        int degree = 0;
        while (first != last) {
            Polynomial_[degree++] = *first++;
        }
        this->Normalize();
    }

    void Normalize() {
        for (auto it = Polynomial_.begin(); it != Polynomial_.end(); ) {
            if (it->second == T(0))
                it = Polynomial_.erase(it);
            else
                ++it;
        }
    }

    int Degree() const {
        if (Polynomial_.empty()) {
            return -1;
        }
        return Polynomial_.rbegin()->first;
    }

    T operator[] (int degree) const {
        try {
            return Polynomial_.at(degree);
        } catch(...) {
            return T(0);
        }
    }

    T operator() (const T& x) const {
        T ans = T(0);
        for (int i = this->Degree(); i >= 0; --i) {
            ans = ans * x + (*this)[i];
        }
        return ans;
    }

    Polynomial& operator+= (const Polynomial& q) {
        int degree = std::max(this->Degree(), q.Degree());
        for (int i = 0; i <= degree; ++i) {
            Polynomial_[i] += q[i];
        }
        this->Normalize();
        return *this;
    }

    Polynomial& operator-= (const Polynomial& q) {
        int degree = std::max(this->Degree(), q.Degree());
        for (int i = 0; i <= degree; ++i) {
            Polynomial_[i] -= q[i];
        }
        this->Normalize();
        return *this;
    }

    Polynomial& operator*= (const Polynomial& x) {
        int degree = this->Degree() + x.Degree();
        std::vector<T> monoms;
        for (int i = 0; i <= degree; ++i) {
            monoms.push_back(T(0));
            for (int j = 0; j < i + 1; ++j) {
                monoms[i] += (*this)[j] * x[i - j];
            }
        }
        *this = Polynomial(monoms);
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
        for (int i = 0; i <= p.Degree(); ++i) {
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
    std::map<int, T> Polynomial_;
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
