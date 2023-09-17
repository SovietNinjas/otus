#include "ip_address.h"

#include <charconv>
#include <iostream> // (a.f.lutfullin) remove

IpAddress::IpAddress(std::string&& str) : asString_(std::move(str)) {}

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
void IpAddress::set(std::string_view strV) {
    std::size_t buffInx = 0;
    for (auto pos = strV.find_first_of('.'); pos != strV.npos; strV.remove_prefix(pos), pos = strV.find_first_of('.')) {
        std::from_chars(cbegin(strV), cbegin(strV) + pos, asNumeric_[buffInx++]);
    }

    for (const auto& i : asNumeric_) {
        std::cout << i << '.';
    }
    std::cout << std::endl;
}
