#include <iostream>
#include <array>

int main(int, char**) {
    std::array<std::pair<int, int>, 3> buff;
    std::cin >> buff[0].first >> buff[0].second >> buff[1].first >> buff[1].second >> buff[2].first >> buff[2].second;

    auto strawberriesHeight = buff[1].first - buff[2].first;

    auto second = strawberriesHeight - (buff[0].first - buff[2].first);
    auto first = strawberriesHeight - second;

    std::cout << first << ' ' << second << std::endl;

    return 0;
}
