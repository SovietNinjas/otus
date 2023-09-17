#pragma once

#include <cstdint>
#include <string>

class IpAddresss {
   public:
    using IpByte = uint8_t;

   private:
    IpByte b1_ = 0;
    IpByte b2_ = 0;
    IpByte b3_ = 0;
    IpByte b4_ = 0;
    std::string asString_{};
};
