#include <cstdint>
#include <iostream>

int main(int, char**) {
    int a, b, c;

    std::cin >> a >> b >> c;

//    if (b != 0) {
//        if (c == 1) {
//            std::cout << a - b - c << std::endl;
//        } else {
//            std::cout << a - b * c << std::endl;
//        }
//    } else {
//        std::cout << -c << std::endl;
//    }

    std::cout << std::min(a - b * c, a - b - c) << std::endl;

    return 0;
}
