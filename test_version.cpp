#define BOOST_TEST_MODULE test_version

#include <gtest/gtest.h>

#include "matrix.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(SuiteName, Name) {
    // бесконечная матрица int заполнена значениями -1
    Matrix<int, -1> matrix;
    EXPECT_EQ(matrix.size(), 0);  // все ячейки свободны

    auto a = matrix[0][0];

    EXPECT_EQ(a, -1);
    EXPECT_EQ(matrix.size(), 0);

    matrix[100][100] = 314;

    EXPECT_EQ(matrix[100][100], 314);
    EXPECT_EQ(matrix.size(), 1);

    ((matrix[100][100] = 314) = 0) = 217;

    // выведется одна строка
    // 100100217
    std::stringstream ss;
    for (auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        ss << x << y << v;
    }

    EXPECT_EQ(ss.str(), "100100217");
}
