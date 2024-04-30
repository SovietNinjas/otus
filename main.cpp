#include <filesystem>
#include <iostream>

#include "mapreduce.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    const auto printError = []() {
        std::cout << "Invalid arguments!" << std::endl
                  << "  Example: mapreduce <src_file> <m_num> <r_num>" << std::endl
                  << "  OR" << std::endl
                  << "  mapreduce some_file.txt 5 5" << std::endl;
    };

    if (argc != 4) {
        printError();
        return EXIT_FAILURE;
    }

    const std::string filePath(argv[1]);
    if (!std::filesystem::exists(filePath)) {
        std::cout << "File \"" << filePath << "\" is not exists" << std::endl;
        return EXIT_FAILURE;
    }

    const auto m = static_cast<unsigned int>(std::atoi(argv[2]));
    const auto r = static_cast<unsigned int>(std::atoi(argv[3]));
    const auto maxThreadCount = std::thread::hardware_concurrency() * 4;

    if ((!m || !r) || (m > maxThreadCount || r > maxThreadCount)) {
        printError();
        return EXIT_FAILURE;
    }

    const auto timePoint = utils::getTick();

    MapReduce mr(m, r, filePath);

    if (!mr.map()) {
        std::cout << mr.getErrorString() << std::endl;
        return EXIT_FAILURE;
    }

    mr.shuffle();

    if (!mr.reduce()) {
        std::cout << mr.getErrorString() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Minimum prefix is " << mr.getMinPrefix_() << std::endl;
    std::cout << "Total time is " << utils::getTickDiff(timePoint) << " msec" << std::endl;
    return EXIT_SUCCESS;
}
