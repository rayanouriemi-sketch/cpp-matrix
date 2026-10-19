#undef NDEBUG  // les assert doivent rester actifs même en Release
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "../src/matrix.hpp"

template <typename F>
static bool throws(F f) {
    try { f(); } catch (const std::exception&) { return true; }
    return false;
}

static bool nearlyEqual(const Matrix<double>& a, const Matrix<double>& b) {
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < a.cols(); ++j)
            if (std::abs(a(i, j) - b(i, j)) > 1e-9) return false;
    return true;
}

int main() {
    Matrix<int> a{{1, 2}, {3, 4}};
    Matrix<int> b{{5, 6}, {7, 8}};

    assert((a + b == Matrix<int>{{6, 8}, {10, 12}}));
    assert((b - a == Matrix<int>{{4, 4}, {4, 4}}));
    assert((a * b == Matrix<int>{{19, 22}, {43, 50}}));
    assert((a * 2 == Matrix<int>{{2, 4}, {6, 8}}));
    assert((a.transpose() == Matrix<int>{{1, 3}, {2, 4}}));
    assert(a * Matrix<int>::identity(2) == a);

    // produit de matrices non carrées : (2x3) x (3x1) = (2x1)
    Matrix<int> c{{1, 2, 3}, {4, 5, 6}};
    Matrix<int> v{{1}, {0}, {-1}};
    assert((c * v == Matrix<int>{{-2}, {-2}}));

    assert(std::abs(a.determinant() - (-2.0)) < 1e-9);
    assert((Matrix<int>{{1, 2}, {2, 4}}).determinant() == 0.0);
    Matrix<double> m{{4, 7, 2}, {3, 6, 1}, {2, 5, 3}};
    assert(nearlyEqual(m * m.inverse(), Matrix<double>::identity(3)));

    // erreurs
    assert(throws([&] { return a + c; }));
    assert(throws([&] { return a * c.transpose(); }));
    assert(throws([&] { return c.determinant(); }));
    assert(throws([&] { return (Matrix<int>{{1, 2}, {2, 4}}).inverse(); }));
    assert(throws([&] { return a(2, 0); }));
    assert(throws([] { return Matrix<int>{{1, 2}, {3}}; }));

    std::ostringstream os;
    os << a;
    assert(os.str() == "[ 1 2 ]\n[ 3 4 ]\n");

    std::cout << "Tous les tests passent.\n";
    return 0;
}
