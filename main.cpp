#include <iostream>

#include "cmd_mgr.h"

int main(int, char* argv[]) {
    std::size_t querryCount = std::stoi(argv[1]);

    FileLogger fileLogger;
    ConsoleLogger consoleLogger;

    CommandMgr cmdMgr(querryCount);
    cmdMgr.addObserver(&fileLogger);
    cmdMgr.addObserver(&consoleLogger);

    for (std::string cmd; std::cin >> cmd;) {
        cmdMgr.pushCmd(std::move(cmd));
    }

    return 0;
}
