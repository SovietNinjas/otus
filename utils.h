#pragma once

#include <algorithm>
#include <chrono>
#include <string>
#include <vector>

namespace utils {
using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

std::vector<std::string> dirFiles(const std::string& dirPath, const std::vector<std::string>& excludeDirList,
                                  const std::vector<std::string>& maskList, bool isRecursive = false);

std::string md5(const std::string& s);

TimePoint getTick();

uint64_t getTickDiff(const TimePoint& p);

void stringToLower(std::string& s);

template <typename T>
bool vectorContains(const std::vector<T>& v, T val) {
    return std::find(v.begin(), v.end(), val) != v.end();
}
}  // namespace utils
