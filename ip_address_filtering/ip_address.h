#pragma once

#include <array>
#include <cstdint>
#include <string>

class IpAddress {
   public:
    IpAddress(std::string&& str);
    static constexpr std::size_t kIpSize = 4;
    using IpBuff = std::array<uint8_t, kIpSize>;
    void set(std::string_view strV);
    inline std::string_view get() const { return asString_; }

   private:
    IpBuff asNumeric_{};
    std::string asString_{};
};
