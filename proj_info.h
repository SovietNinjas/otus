#pragma once

#include <string_view>

namespace project_info {

int version();

std::string_view getProjectName();

void printProjectInfo();

}  // namespace lib
