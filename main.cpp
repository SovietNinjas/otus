#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>

template <typename T, typename = std::enable_if_t<std::is_same_v<T, std::vector<typename T::value_type>> ||
                                                  std::is_same_v<T, std::list<typename T::value_type>>>>
void print_ip(T container) {
    auto it = cbegin(container);
    std::cout << *it++;
    std::for_each(it, cend(container), [](auto val) { std::cout << '.' << val; });
    std::cout << std::endl;
}

template <std::integral T>
void print_ip(T ip) {
    std::vector<int> res(sizeof(T));

    std::for_each(rbegin(res), rend(res), [&ip](auto& val) {
        val = static_cast<uint8_t>(ip);
        ip >>= 8;
    });

    print_ip(res);
}

void print_ip(const std::string& ip) { std::cout << ip << std::endl; }

template <typename A, typename B>
struct TypeAssert {
    static_assert(std::is_same_v<A, B>);
};

template <typename... U>
void print_ip(std::tuple<U...> t) {
    using ElemType = std::tuple_element_t<0, decltype(t)>;
    [[maybe_unused]] auto typeAssert = {(TypeAssert<ElemType, U>{})...};

    std::vector<int> res;

    std::apply([&res](const auto&... elems) { res = {(elems)...}; }, t);

    print_ip(res);
}

int main(int, char**) {
    print_ip(int8_t{-1});                            // 255
    print_ip(int16_t{0});                            // 0.0
    print_ip(int32_t{2130706433});                   // 127.0.0.1
    print_ip(int64_t{8875824491850138409});          // 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"});          // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400});  // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100});  // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0));  // 123.456.789.0

    return 0;
}
