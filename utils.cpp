#include "utils.h"

std::string utils::getSocketAddress(std::shared_ptr<Session> s) {
    auto rmt = s->getSocket().remote_endpoint();
    return rmt.address().to_string() + ":" + std::to_string(rmt.port());
}

std::vector<std::string> utils::splitString(const std::string& s, char sep) {
    std::vector<std::string> v;

    if (!s.empty()) {
        size_t pos = 0, lastPos = 0;
        while ((pos = s.find(sep, lastPos)) != std::string::npos) {
            if (pos != 0) {
                if ((pos - lastPos) > 0) {
                    v.emplace_back(s.substr(lastPos, pos - lastPos));
                }
                lastPos = ++pos;
            } else {
                ++lastPos;
            }
        }

        if (pos == std::string::npos) {
            size_t Size = s.size();
            if (lastPos < Size) {
                v.emplace_back(s.substr(lastPos, Size - lastPos));
            }
        }
    }

    return v;
}

void utils::stringToLower(std::string& s) {
    for (size_t i = 0, c = s.size(); i < c; ++i) {
        s[i] = (char)std::tolower((int)s[i]);
    }
}

std::optional<uint64_t> utils::stringToUint64(const std::string& s) {
    try {
        return std::stoull(s);
    } catch (const std::exception&) {
    }

    return std::nullopt;
}

void utils::stringRmRight(std::string& s, size_t n) {
    size_t size = s.size();
    if (size == 0) {
        return;
    }

    if (n >= size) {
        s.clear();
    } else {
        s.erase(size - n);
    }
}
