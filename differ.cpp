#include "differ.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "utils.h"

Differ::Differ(const std::vector<std::string>& dirPathList, const std::vector<std::string>& excludeDirList,
               const std::vector<std::string>& maskList, bool isRecursive, unsigned int minSize,
               uint64_t chunk_size)
    : pathDirList_(dirPathList),
      excludeDirList_(excludeDirList),
      maskList_(maskList),
      isRecursive_(isRecursive),
      minSize_(minSize),
      chunkSize_(chunk_size) {}

const std::string& Differ::getErrorString() const { return errorString_; }

bool Differ::init() {
    if (pathDirList_.empty()) {
        errorString_ = "The list of path is empty";
        return false;
    }

    for (const std::string& dir_path : pathDirList_) {
        if (!std::filesystem::is_directory(dir_path)) {
            errorString_ = "The path \"" + dir_path + "\" is not a directory";
            return false;
        }
    }

    for (std::string& s : maskList_) {
        utils::stringToLower(s);
    }

    return true;
}

bool Differ::run() {
    auto t = utils::getTick();
    std::vector<std::string> files;
    for (const std::string& dir_path : pathDirList_) {
        auto f = utils::dirFiles(dir_path, excludeDirList_, maskList_, isRecursive_);
        files.insert(files.end(), f.begin(), f.end());
    }

    if (auto s = files.size(); s < 2) {
        errorString_ = "In total your directories has less than two files (" + std::to_string(s);
        return false;
    }
    uint64_t read_dir_ms = utils::getTickDiff(t);

    t = utils::getTick();

    for (auto file_left = files.begin(); file_left != files.end(); ++file_left) {
        uint64_t file_left_size = std::filesystem::file_size(*file_left);
        if (file_left_size < minSize_) {
            continue;
        }

        std::ifstream file(*file_left);
        if (!file.is_open()) {
            throw std::runtime_error("Can't open file " + *file_left);
        }

        for (auto file_other = files.begin(); file_other != files.end(); ++file_other) {
            if (std::filesystem::file_size(*file_other) != file_left_size || file_left == file_other) {
                continue;
            }

            if (diff_(file, *file_other, file_left_size)) {
                insertToMap_(*file_left, *file_other);
            }
        }
    }

    uint64_t process_ms = utils::getTickDiff(t);

    printResult_();

    std::cout << "Read the directory time: " << read_dir_ms << " msec" << std::endl;
    std::cout << "Diff process time:       " << process_ms << " msec" << std::endl;

    return true;
}

bool Differ::diff_(std::ifstream& file, const std::string& file_path_right, uint64_t size) {
    std::ifstream file_right(file_path_right);
    if (!file_right.is_open()) {
        throw std::runtime_error("Can't open file " + file_path_right);
    }

    std::string c1;
    std::string c2;
    uint64_t cnt = 0;
    bool result = false;

    while (size > 0) {
        cnt = size < chunkSize_ ? size : chunkSize_;

        file.read(&c1[0], cnt);
        file_right.read(&c2[0], cnt);
        size -= cnt;

        result = utils::md5(c1) == utils::md5(c2);
        if (!result) {
            break;
        }
    }

    return result;
}

void Differ::insertToMap_(const std::string& file_left, const std::string& file_right) {
    bool already_exists = false;
    for (const auto& map_item : mapSimilar_) {
        already_exists = std::find(map_item.second.begin(), map_item.second.end(), file_left) != map_item.second.end();
        if (already_exists) {
            break;
        }
    }

    if (!already_exists) {
        mapSimilar_[file_left].emplace_back(file_right);
    }
}

void Differ::printResult_() {
    for (const auto& map_item : mapSimilar_) {
        std::cout << map_item.first << std::endl;
        for (const std::string& file_path : map_item.second) {
            std::cout << file_path << std::endl;
        }
        std::cout << std::endl;
    }
}
