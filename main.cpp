#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>

int main(int, char**) {
    auto i = 0;

    while (std::cin >> i) {
        if (i >= 1 && i <= 4) {
            std::cout << "few" << std::endl;
        } else if (i >= 5 && i <= 9) {
            std::cout << "several" << std::endl;
        } else if (i >= 10 && i <= 19) {
            std::cout << "pack" << std::endl;
        } else if (i >= 20 && i <= 49) {
            std::cout << "lots" << std::endl;
        } else if (i >= 50 && i <= 99) {
            std::cout << "horde" << std::endl;
        } else if (i >= 100 && i <= 249) {
            std::cout << "throng" << std::endl;
        } else if (i >= 250 && i <= 499) {
            std::cout << "swarm" << std::endl;
        } else if (i >= 500 && i <= 999) {
            std::cout << "zounds" << std::endl;
        } else if (i >= 1000) {
            std::cout << "legion" << std::endl;
        }
    }

    return 0;
}
