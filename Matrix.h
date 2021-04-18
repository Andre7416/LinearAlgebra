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
class MatrixIterator;

template<typename T>
class ConstMatrixIterator;

template<typename T>
class Matrix {
public:
    using iterator = MatrixIterator<T>;
    using const_iterator = ConstMatrixIterator<T>;

    Matrix(const std::vector<std::vector<T>>& matrix) {
        Matrix_ = matrix;
    }

    std::pair<size_t, size_t> size() const {
        return {Matrix_.size(), Matrix_[0].size()};
    }

    std::vector<T>& operator[] (size_t i) {
        return Matrix_[i];
    }

    const std::vector<T>& operator[] (size_t i) const {
        return Matrix_[i];
    }

    Matrix& operator+= (const Matrix& other) {
        for (size_t i = 0; i < this->size().first; ++i) {
            for (size_t j = 0; j < this->size().second; ++j) {
                this->Matrix_[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix operator+ (const Matrix& other) const {
        Matrix result = *this;
        result += other;
        return result;
    }

    template<typename N>
    Matrix& operator*= (const N& number) {
        for (size_t i = 0; i < this->size().first; ++i) {
            for (size_t j = 0; j < this->size().second; ++j) {
                this->Matrix_[i][j] *= number;
            }
        }
        return *this;
    }

    template<typename N>
    Matrix operator* (const N& number) const {
        Matrix result = *this;
        result *= number;
        return result;
    }

    Matrix& transpose() {
        *this = this->transposed();
        return *this;
    }

    Matrix transposed() const {
        std::vector<std::vector<T>> matrix;
        matrix.resize(this->size().second);
        for (size_t i = 0; i < this->size().second; ++i) {
            matrix[i].resize(this->size().first);
        }
        for (size_t i = 0; i < this->size().first; ++i) {
            for (size_t j = 0; j < this->size().second; ++j) {
                matrix[j][i] = Matrix_[i][j];
            }
        }
        return matrix;
    }

    Matrix& operator*= (const Matrix& other) {
        assert(this->size().second == other.size().first);
        std::vector<std::vector<T>> matrix;
        matrix.resize(this->size().first);
        for (size_t i = 0; i < this->size().first; ++i) {
            matrix[i].resize(other.size().second);
        }
        for (size_t i = 0; i < this->size().first; ++i) {
            for (size_t j = 0; j < other.size().second; ++j) {
                T x(0);
                for (size_t k = 0; k < other.size().first; ++k) {
                    x += (*this)[i][k] * other[k][j];
                }
                matrix[i][j] = x;
            }
        }
        this->Matrix_ = matrix;
        return *this;
    }

    Matrix operator* (const Matrix& other) const {
        Matrix result = *this;
        return result *= other;
    }

    iterator begin() {
        iterator it(this);
        return it.begin();
    }

    iterator end() {
        iterator it(this);
        return it.end();
    }
    const_iterator end() const {
        const_iterator it(this);
        return it.end();
    }

    const_iterator begin() const {
        const_iterator it(this);
        return it.begin();
    }



private:
    std::vector<std::vector<T>> Matrix_;
};

template<typename T>
std::ostream& operator<< (std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.size().first; ++i) {
        for (size_t j = 0; j < m.size().second; ++j) {
            out << m[i][j];
            if (j != m.size().second - 1) {
                out << "\t";
            }
        }
        if (i != m.size().first - 1) {
            out << "\n";
        }
    }
    return out;
}

template<typename T>
class MatrixIterator  {
public:
    MatrixIterator(Matrix<T>* matrix): Matrix_(matrix) {
    }

    MatrixIterator& operator++ () {
        ++m;
        if (m == Matrix_->size().second) {
            m = 0;
            ++n;
        }
        return *this;
    }
    MatrixIterator operator++ (int) {
        MatrixIterator temp = *this;
        ++m;
        if (m == Matrix_->size().second) {
            m = 0;
            ++n;
        }
        return temp;
    }

    template<typename N>
    friend bool operator== (const MatrixIterator<N>& it, const MatrixIterator<N>& other);

    template<typename N>
    friend bool operator!= (const MatrixIterator<N>& it, const MatrixIterator<N>& other);

    T& operator* () {
        return (*Matrix_)[n][m];
    }

    MatrixIterator& begin() {
        n = 0;
        m = 0;
        return *this;
    }
    MatrixIterator& end() {
        n = Matrix_->size().first;
        m = 0;
        return *this;
    }

private:
    Matrix<T>* Matrix_;
    size_t m = 0, n = 0;
};

template<typename T>
bool operator== (const MatrixIterator<T>& it, const MatrixIterator<T>& other) {
    return it.n == other.n && it.m == other.m;
}

template<typename T>
bool operator!= (const MatrixIterator<T>& it, const MatrixIterator<T>& other) {
    return !(it == other);
}

template<typename T>
class ConstMatrixIterator  {
public:
    ConstMatrixIterator(const Matrix<T>* matrix) {
        Matrix_ = matrix;
    }

    ConstMatrixIterator& operator++ () {
        ++m;
        if (m == Matrix_->size().second) {
            m = 0;
            ++n;
        }
        return *this;
    }
    ConstMatrixIterator operator++ (int) {
        ConstMatrixIterator temp = *this;
        ++m;
        if (m == Matrix_->size().second) {
            m = 0;
            ++n;
        }
        return temp;
    }

    template<typename N>
    friend bool operator== (const ConstMatrixIterator<N>& it, const ConstMatrixIterator<N>& other);

    template<typename N>
    friend bool operator!= (const ConstMatrixIterator<N>& it, const ConstMatrixIterator<N>& other);

    T operator* () {
        return (*Matrix_)[n][m];
    }

    ConstMatrixIterator& begin() {
        n = 0;
        m = 0;
        return *this;
    }
    ConstMatrixIterator& end() {
        n = Matrix_->size().first;
        m = 0;
        return *this;
    }

private:
    const Matrix<T>* Matrix_;
    size_t m = 0, n = 0;
};

template<typename T>
bool operator== (const ConstMatrixIterator<T>& it, const ConstMatrixIterator<T>& other) {
    return it.n == other.n && it.m == other.m;
}

template<typename T>
bool operator!= (const ConstMatrixIterator<T>& it, const ConstMatrixIterator<T>& other) {
    return !(it == other);
}
