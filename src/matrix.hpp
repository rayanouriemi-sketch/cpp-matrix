#pragma once

#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

// Matrice dense générique, stockée ligne par ligne.
template <typename T>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols, T value = T{})
        : rows_(rows), cols_(cols), data_(rows * cols, value) {}

    Matrix(std::initializer_list<std::initializer_list<T>> init)
        : rows_(init.size()), cols_(init.size() ? init.begin()->size() : 0) {
        data_.reserve(rows_ * cols_);
        for (const auto& row : init) {
            if (row.size() != cols_) throw std::invalid_argument("lignes de tailles différentes");
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }

    static Matrix identity(std::size_t n) {
        Matrix m(n, n);
        for (std::size_t i = 0; i < n; ++i) m(i, i) = T{1};
        return m;
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    T& operator()(std::size_t r, std::size_t c) { return data_[index(r, c)]; }
    const T& operator()(std::size_t r, std::size_t c) const { return data_[index(r, c)]; }

    Matrix operator+(const Matrix& o) const { return combine(o, [](T a, T b) { return a + b; }); }
    Matrix operator-(const Matrix& o) const { return combine(o, [](T a, T b) { return a - b; }); }

    Matrix operator*(const Matrix& o) const {
        if (cols_ != o.rows_) throw std::invalid_argument("dimensions incompatibles pour le produit");
        Matrix result(rows_, o.cols_);
        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t k = 0; k < cols_; ++k)  // ordre i-k-j : accès mémoire contigus
                for (std::size_t j = 0; j < o.cols_; ++j)
                    result(i, j) += (*this)(i, k) * o(k, j);
        return result;
    }

    Matrix operator*(T scalar) const {
        Matrix result = *this;
        for (auto& v : result.data_) v *= scalar;
        return result;
    }

    bool operator==(const Matrix& o) const {
        return rows_ == o.rows_ && cols_ == o.cols_ && data_ == o.data_;
    }
    bool operator!=(const Matrix& o) const { return !(*this == o); }

    Matrix transpose() const {
        Matrix result(cols_, rows_);
        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t j = 0; j < cols_; ++j) result(j, i) = (*this)(i, j);
        return result;
    }

    // Déterminant par élimination de Gauss avec pivot partiel, O(n^3).
    double determinant() const {
        requireSquare();
        std::vector<double> a(data_.begin(), data_.end());
        const std::size_t n = rows_;
        double det = 1.0;
        for (std::size_t col = 0; col < n; ++col) {
            std::size_t pivot = col;
            for (std::size_t r = col + 1; r < n; ++r)
                if (std::abs(a[r * n + col]) > std::abs(a[pivot * n + col])) pivot = r;
            if (std::abs(a[pivot * n + col]) < 1e-12) return 0.0;
            if (pivot != col) {
                for (std::size_t c = 0; c < n; ++c) std::swap(a[col * n + c], a[pivot * n + c]);
                det = -det;
            }
            det *= a[col * n + col];
            for (std::size_t r = col + 1; r < n; ++r) {
                double factor = a[r * n + col] / a[col * n + col];
                for (std::size_t c = col; c < n; ++c) a[r * n + c] -= factor * a[col * n + c];
            }
        }
        return det;
    }

    // Inverse par Gauss-Jordan. Lève une exception si la matrice est singulière.
    Matrix<double> inverse() const {
        requireSquare();
        const std::size_t n = rows_;
        Matrix<double> a(n, n), inv = Matrix<double>::identity(n);
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j) a(i, j) = static_cast<double>((*this)(i, j));

        for (std::size_t col = 0; col < n; ++col) {
            std::size_t pivot = col;
            for (std::size_t r = col + 1; r < n; ++r)
                if (std::abs(a(r, col)) > std::abs(a(pivot, col))) pivot = r;
            if (std::abs(a(pivot, col)) < 1e-12) throw std::domain_error("matrice non inversible");
            for (std::size_t c = 0; c < n; ++c) {
                std::swap(a(col, c), a(pivot, c));
                std::swap(inv(col, c), inv(pivot, c));
            }
            double p = a(col, col);
            for (std::size_t c = 0; c < n; ++c) { a(col, c) /= p; inv(col, c) /= p; }
            for (std::size_t r = 0; r < n; ++r) {
                if (r == col) continue;
                double f = a(r, col);
                for (std::size_t c = 0; c < n; ++c) { a(r, c) -= f * a(col, c); inv(r, c) -= f * inv(col, c); }
            }
        }
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rows_; ++i) {
            os << "[ ";
            for (std::size_t j = 0; j < m.cols_; ++j) os << m(i, j) << ' ';
            os << "]\n";
        }
        return os;
    }

private:
    std::size_t index(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("indice hors de la matrice");
        return r * cols_ + c;
    }

    void requireSquare() const {
        if (rows_ != cols_) throw std::invalid_argument("la matrice doit être carrée");
    }

    template <typename Op>
    Matrix combine(const Matrix& o, Op op) const {
        if (rows_ != o.rows_ || cols_ != o.cols_) throw std::invalid_argument("dimensions différentes");
        Matrix result(rows_, cols_);
        for (std::size_t i = 0; i < data_.size(); ++i) result.data_[i] = op(data_[i], o.data_[i]);
        return result;
    }

    std::size_t rows_, cols_;
    std::vector<T> data_;
};
