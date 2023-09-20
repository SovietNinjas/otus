#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "ip_address.h"

template <typename T>
void printRng(const T& buff) {
    for (const auto& i : buff) {
        std::cout << i << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, IpAddress addr) {
    for (auto pos = cbegin(addr.asNumeric_), end = cend(addr.asNumeric_); pos != end; ++pos) {
        if (pos != cbegin(addr.asNumeric_)) {
            os << '.';
        }
        os << static_cast<int>(*pos);
    }
    return os;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
    //    project_info::printProjectInfo();

    auto reverseIpComp = [](IpAddress lhs, IpAddress rhs) { return lhs.asNumeric_ > rhs.asNumeric_; };

    std::multiset<IpAddress, decltype(reverseIpComp)> ipAddresses;
    std::vector<IpAddress> ipAddressesFistByte1;
    std::vector<IpAddress> ipAddressesFirstByte46Second70;
    std::vector<IpAddress> ipAddressesAnyByte46;

    try {
        //        size_t count = 0;
        for (std::string line; std::getline(std::cin, line);) {
            std::string_view ipAddr(line);
            ipAddresses.emplace(ipAddr.substr(0, line.find_first_of('\t')));
            // if (++count > 3) break;
        }

        for (const auto& ip : ipAddresses) {
            for (auto pos = cbegin(ip.asNumeric_), end = cend(ip.asNumeric_); pos != end; ++pos) {
                if (pos != cbegin(ip.asNumeric_)) {
                    std::cout << '.';
                    if (*pos == 46) {
                        ipAddressesAnyByte46.push_back(ip);
                    }
                } else {
                    if (*pos == 46) {
                        ipAddressesAnyByte46.push_back(ip);
                        if (pos[1] == 70) {
                            ipAddressesFirstByte46Second70.push_back(ip);
                        }
                    } else if (*pos == 1) {
                        ipAddressesFistByte1.push_back(ip);
                    }
                }
                std::cout << static_cast<int>(*pos);
            }
            std::cout << std::endl;
        }

        printRng(ipAddressesFistByte1);
        printRng(ipAddressesFirstByte46Second70);
        printRng(ipAddressesAnyByte46);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
