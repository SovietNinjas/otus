#pragma once

#include <algorithm>
#include <string_view>

#include "allocator.h"

namespace utils {

inline unsigned int factorial(unsigned int n) {
    if (n == 0) {
        return 1;
    }

    return n * factorial(n - 1);
}

template <typename T>
struct type_false : std::false_type {};

template <typename T>
void fill(T& t) {
    if constexpr (std::is_same_v<std::map<int, int>, T> ||
                  std::is_same_v<std::map<int, int, std::less<int>, custom::Allocator<std::pair<const int, int>>>, T>) {
        for (unsigned int i = 0; i < 10; ++i) {
            t[i] = factorial(i);
        }
    } else if constexpr (std::is_same_v<custom::List<int>, T> ||
                         std::is_same_v<custom::List<int, custom::Allocator<int>>, T>) {
        for (unsigned int i = 0; i < 10; ++i) {
            t.pushFront(i);
        }
    } else if constexpr (std::is_same_v<std::vector<int, custom::Allocator<int>>, T>) {
        for (unsigned int i = 0; i < 10; ++i) {
            t.emplace_back(i);
        }
    } else {
        static_assert(type_false<T>::value, "Not support template type");
    }
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<const K, V>& p) {
    os << p.first << ' ' << p.second;
    return os;
}

template <typename Cont>
void print(const Cont& cont, std::string_view comment) {
    std::cout << comment << '\n';
    for (const auto& i : cont) {
        std::cout << i << '\n';
    }
    std::cout << std::endl;
}

}  // namespace utils
