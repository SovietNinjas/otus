#include <iostream>

#include "matrix.h"

int main(int, char**) {
    Matrix<int, 0> matrix{};

    for (std::size_t i = 0, j = 0, y = 9; i < 10; ++i, ++j, --y) {
        matrix[i][j] = i;
        matrix[i][y] = y;
    }

    for (std::size_t i = 1; i < 9; ++i) {
        for (std::size_t j = 1; j < 9; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nmatrix size : " << matrix.size() << "\n\n";

    for (const auto&[x,y,v] : matrix) {
        std::cout << '(' << x << ", " << y << ") = " << v << std::endl;
    }

    return 0;
}
