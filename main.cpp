#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

#include "differ.h"

int main(int argc, char** argv) {
    namespace po = boost::program_options;
    using ValueT = std::vector<std::string>;

    bool is_recursive = false;
    unsigned int minimum_size = 1;
    uint64_t chunk_size = 5;

    po::options_description desc("Options");

    desc.add_options()("help", "Print this message")("dirs", po::value<ValueT>()->multitoken()->required(),
                                                     "List of directories for the scan")(
        "exclude", po::value<ValueT>()->multitoken(), "List of directories for exclude from the scan")(
        "mask", po::value<ValueT>()->multitoken(), "List of masks for allowed file names")(
        "recursive", po::bool_switch(&is_recursive), "Is recursive scan. Default: false")(
        "minimum-size", po::value(&minimum_size), "Minimum size of file for diff. Default: 1 byte")(
        "chunk-size", po::value<uint64_t>(&chunk_size), "File reading chunk size. Default: 5 bytes");

    po::variables_map var_map;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), var_map);

    try {
        notify(var_map);
    } catch (const std::exception& e) {
        if (var_map.count("help")) {
            std::cout << std::endl << desc << std::endl;
            return EXIT_SUCCESS;
        }
        std::cout << e.what() << std::endl << std::endl << desc;
        return EXIT_FAILURE;
    }


    try {
        Differ d(var_map.at("dirs").as<ValueT>(), var_map.at("exclude").as<ValueT>(), var_map.at("mask").as<ValueT>(),
                 is_recursive, minimum_size, chunk_size);
        if (!d.init() || !d.run()) {
            std::cout << d.getErrorString() << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
