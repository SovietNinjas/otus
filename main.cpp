#include <cmath>
#include <cstdint>
#include <iostream>

int main(int, char**) {
    int n, k;

    std::cin >> n >> k;

    if (auto res = std::ceil(n * 2. / k); res < 2) {
        std::cout << 2 << std::endl;
    } else {
        std::cout << res << std::endl;
    }

    return 0;
}
