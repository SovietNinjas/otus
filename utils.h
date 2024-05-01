#pragma once

#include <string>
#include <vector>

namespace utils {
std::vector<std::string_view> splitString(std::string_view sv, char sep);
std::string getCurrentTime();
}  // namespace utils
