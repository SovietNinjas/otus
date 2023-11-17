#include <iostream>
#include <numeric>

int main(int, char**) {
    int n, k;

    std::cin >> k >> n;

    int left = 0;
    for (int i = 0; i < n; ++i) {
        int tmp;
        std::cin >> tmp;
        left += tmp - k;
        if (left < 0) {
            left = 0;
        }
    }

    std::cout << left << std::endl;

    return 0;
}
