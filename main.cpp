#include <allocator.h>

#include <iostream>
#include <map>
#include <vector>

#include "proj_info.h"

int main(int, char **) {
    project_info::printProjectInfo();

    std::map<int, int, std::less<int>, custom::Allocator<std::pair<const int, int>>> m{};

    m.emplace(1, 1);

    return 0;
}
