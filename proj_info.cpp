#include "proj_info.h"

#include <iostream>

#include "version.h"

int project_info::version() { return PROJECT_VERSION_PATCH; }

std::string_view project_info::getProjectName() { return PROJECT_NAME; }

void project_info::printProjectInfo() {
    std::cout << "Project : " << project_info::getProjectName() << "\nVersion : " << version() << std::endl;
}
