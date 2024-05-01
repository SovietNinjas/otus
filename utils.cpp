#include "utils.h"

#include <chrono>

std::vector<std::string_view> utils::splitString(std::string_view sv, char sep) {
    std::vector<std::string_view> v;

    if (!sv.empty()) {
        size_t pos = 0, lastPos = 0;
        while ((pos = sv.find(sep, lastPos)) != std::string::npos) {
            if (pos != 0) {
                if ((pos - lastPos) > 0) {
                    v.emplace_back(sv.substr(lastPos, pos - lastPos));
                }
                lastPos = ++pos;
            } else {
                ++lastPos;
            }
        }

        if (pos == std::string::npos) {
            size_t Size = sv.size();
            if (lastPos < Size) {
                v.emplace_back(sv.substr(lastPos, Size - lastPos));
            }
        }
    }

    return v;
}

std::string utils::getCurrentTime() {
    using namespace std::chrono;
    auto time = system_clock::to_time_t(system_clock::now());
    return std::to_string(time);
}
