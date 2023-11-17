#include <iostream>

int main(int, char**) {
    std::size_t querryCount, guestCount;
    std::cin >> querryCount;
    std::cin.ignore(1);
    guestCount = querryCount + 2;

    std::string line;
    while (querryCount) {
        std::getline(std::cin, line);
        if (line.find("+one") != line.npos) {
            ++guestCount;
        }


        --querryCount;
    }

    if (guestCount == 13) {
        ++guestCount;
    }

    std::cout << guestCount * 100 << std::endl;


    return 0;
}
