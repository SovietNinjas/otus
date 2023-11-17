#include <iostream>

bool isAvailable(char a, char b)  {
    return a >= 'a' && a <= 'h' && b >= '1' && b <= '8';
}

int main(int, char**) {
    std::size_t querryCount;
    std::cin >> querryCount;
    std::cin.ignore(1);

    std::string line;
    while (querryCount) {
        std::size_t availableSteps = 0;

        std::getline(std::cin, line);

        if (isAvailable(line[0] + 1, line[1] + 2)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] + 1, line[1] - 2)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] - 1, line[1] + 2)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] - 1, line[1] - 2)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] + 2, line[1] - 1)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] + 2, line[1] + 1)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] - 2, line[1] + 1)) {
            ++availableSteps;
        }
        if (isAvailable(line[0] - 2, line[1] - 1)) {
            ++availableSteps;
        }

        std::cout << availableSteps << '\n';

        --querryCount;
    }
    std::cout << std::flush;

    return 0;
}
