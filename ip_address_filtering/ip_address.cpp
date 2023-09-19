#include "ip_address.h"

#include <charconv>
#include <iostream>  // (a.f.lutfullin) remove

IpAddress::IpAddress(std::string_view str) { set(str); }

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
void IpAddress::set(std::string_view strV) {
    std::string_view::size_type pos = 0;// strV.find_first_of('.');
    for(auto& i : asNumeric_) {
        pos = strV.find_first_of('.');
        std::from_chars(cbegin(strV), cbegin(strV) + pos, i);
        if (pos == strV.npos) break;
        strV.remove_prefix(pos + 1);
    }
}
