#include <array>
#include <iostream>

int main(int, char**) {
    int n, m;

    std::cin >> n >> m;

    if (n % 2 == 0 || m % 2 != 0) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }

    return 0;
}
