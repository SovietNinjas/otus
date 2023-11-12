#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>

static constexpr int maxCount = 4 * 60 / 45;

int main(int, char**) {
    auto i = 0;

    while (std::cin >> i) {
        if (12 - i <= maxCount) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}
