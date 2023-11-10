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
    std::cout << std::fixed << std::setprecision(4);

    printSqrt(std::cin);

    return 0;
}
