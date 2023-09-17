#include "lib.h"

#include <iostream>

#include "version.h"

int project_info::version() { return PROJECT_VERSION_PATCH; }

std::string_view project_info::getProjectName() { return PROJECT_NAME; }

void project_info::printProjectInfo() {
    std::cout << project_info::getProjectName() << "\nversion : " << version() << std::endl;
}
