#pragma once

#include "session.h"

namespace utils
{
    std::string getSocketAddress(std::shared_ptr<Session> s);
    std::vector<std::string> splitString(const std::string& s, char sep);
    void stringToLower(std::string& s);
    std::optional<uint64_t> stringToUint64(const std::string& s);
    void stringRmRight(std::string& s, size_t n);
}

