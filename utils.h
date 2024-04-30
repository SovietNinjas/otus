#pragma once

#include <chrono>
#include <sstream>
#include <string>
#include <thread>

namespace utils {

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

TimePoint getTick();
uint64_t getTickDiff(const TimePoint& t);
std::string getCurrentThreadID();

}  // namespace utils
