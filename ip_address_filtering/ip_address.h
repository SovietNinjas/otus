#pragma once

#include <array>
#include <cstdint>
#include <string>

class IpAddress {
   public:
    static constexpr std::size_t kIpSize = 4;
    using IpBuff = std::array<uint8_t, kIpSize>;

    IpAddress(std::string_view str);
    void set(std::string_view strV);
    IpBuff asNumeric_{};
};
