#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main(int, char**) {
    std::size_t count = 0;

    std::vector<std::vector<std::size_t>> nums;
    nums.reserve(3);

    for (std::size_t i = 0; i < 3; ++i) {
        std::size_t numCount;
        std::cin >> numCount;
        std::cin.ignore(1);

        std::string line;
        std::getline(std::cin, line);
        std::istringstream is(std::move(line));
        nums.push_back(std::vector<std::size_t>(std::istream_iterator<std::size_t>(is), std::istream_iterator<std::size_t>()));
    }

    for (const auto& i : nums[0]) {
        auto it = std::lower_bound(begin(nums[1]), end(nums[1]), i);
        if (it != nums[1].end() && *it == i) {
            it = std::lower_bound(begin(nums[2]), end(nums[2]), i);
            if (it != nums[2].end() && *it == i) {
                ++count;
            }
        }
    }

    std::cout << count << std::endl;

    return 0;
}
