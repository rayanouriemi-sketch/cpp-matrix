#include <iostream>

#include "matrix.hpp"

int main() {
    Matrix<double> a{{2, 1, 1}, {1, 3, 2}, {1, 0, 0}};
    Matrix<double> b{{1, 2}, {0, 1}, {4, 0}};

    std::cout << "A =\n" << a << "\nB =\n" << b;
    std::cout << "\nA x B =\n" << a * b;
    std::cout << "\ntranspose(B) =\n" << b.transpose();
    std::cout << "\ndet(A) = " << a.determinant() << "\n";
    std::cout << "\ninverse(A) =\n" << a.inverse();
    std::cout << "\nA x inverse(A) =\n" << a * a.inverse();
    return 0;
}
