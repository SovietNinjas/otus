#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>

void printSqrt(std::istream& is) {
    uint64_t res = 0;
    if (is >> res) {
        printSqrt(is);
        std::cout << std::sqrt(res) << std::endl;
    }
}

int main(int, char**) {
    int a, b, c;

    std::cin >> a >> b >> c;
    std::cout << a * b * c * 2 << std::endl;

    return 0;
}
