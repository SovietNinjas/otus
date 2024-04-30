#include "mapreduce.h"

#include <string.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.h"

MapReduce::MapReduce(unsigned int m, unsigned int r, const std::string& filePath)
    : map_(m),
      reduce_(r),
      filePath_(filePath),
      dirOutput_(std::filesystem::path(filePath).parent_path() / "output"),
      activeThread_(0),
      minPrefix_(0) {}

const std::string& MapReduce::getErrorString() const { return errorString_; }

size_t MapReduce::getMinPrefix_() const { return minPrefix_; }

bool MapReduce::map() {
    if (!split_()) {
        return false;
    }

    activeThread_ = (unsigned int)chunks_.size();
    for (std::string& chunk : chunks_) {
        std::thread(&MapReduce::workerMap_, this, std::ref(chunk)).detach();
    }

    while (activeThread_ > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return true;
}

void MapReduce::shuffle() {
    if (vectorTotal_.size() > 1) {
        auto& firstMap = vectorTotal_.front();

        for (size_t i = 1; i < vectorTotal_.size(); ++i) {
            auto& currentMap = vectorTotal_[i];
            firstMap.insert(currentMap.begin(), currentMap.end());
            currentMap.clear();
        }

        vectorTotal_.resize(1);
    }
}

bool MapReduce::reduce() {
    if (!prepareOutputDir_()) {
        return false;
    }

    auto& firstMap = vectorTotal_.front();

    std::vector<std::string> v;
    v.resize(firstMap.size());

    int i = -1;
    for (const auto& a : firstMap) {
        v[++i] = a.first;
    }

    unsigned int vSize = v.size();
    unsigned int chunkSize = vSize / reduce_;
    unsigned int offset = 0;
    activeThread_ = reduce_ + (vSize % reduce_ > 0 ? 1 : 0);
    int fileIndex = 0;

    while (vSize > 0) {
        if (vSize < chunkSize) {
            chunkSize = vSize;
        }

        std::thread(&MapReduce::workerReduce_, this,
                    std::vector<std::string>(v.begin() + offset, v.begin() + chunkSize + offset), ++fileIndex)
            .detach();

        vSize -= chunkSize;
        offset += chunkSize;
    }

    while (activeThread_ > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return true;
}

bool MapReduce::split_() {
    std::error_code e;
    uintmax_t fileSize = std::filesystem::file_size(filePath_, e);
    if (e) {
        errorString_ = e.message();
        return false;
    }

    std::cout << "Start reading file " << filePath_ << std::endl;

    std::ifstream file(filePath_);
    if (!file.is_open()) {
        errorString_ = strerror(errno);
        return false;
    }

    size_t chunkSize = (size_t)fileSize / map_;
    size_t pos = 0;
    auto timePoint = utils::getTick();

    std::vector<char> vec(chunkSize);
    while (file.read(&vec[0], chunkSize)) {
        std::cout << "Reading " << std::to_string(chunks_.size() + 1) << " chunk..." << std::endl;

        processVector_(vec);

        pos += vec.size();
        file.seekg(pos);
    }

    processVector_(vec);

    std::cout << "Read OK by " << utils::getTickDiff(timePoint) << " msec" << std::endl;
    return true;
}

void MapReduce::workerMap_(std::string& s) {
    std::string* threadId = new std::string(utils::getCurrentThreadID());
    printf("%s\tstarted thread\n", threadId->c_str());

    size_t reserveSize = 0;
    for (size_t i = 0, c = s.size(); i < c; ++i) {
        if (s[i] == '\n') {
            ++reserveSize;
        }
    }

    std::istringstream stream(s);
    std::vector<std::string> v;
    v.reserve(reserveSize);

    auto timePoint = utils::getTick();

    std::string line;
    while (std::getline(stream, line)) {
        auto lineSize = line.size();
        if (lineSize == 0) {
            continue;
        }

        if (minPrefix_ == 0 || lineSize < minPrefix_) {
            minPrefix_ = lineSize;
        }

        v.emplace_back(std::move(line));
    }

    s.clear();
    s.shrink_to_fit();

    auto strings = getStringByMinSize_(v);

    mutex_.lock();
    vectorTotal_.emplace_back(strings);
    --activeThread_;
    mutex_.unlock();

    printf("%s\tfinished thread by %lu msec\n", threadId->c_str(), utils::getTickDiff(timePoint));
}

void MapReduce::workerReduce_(const std::vector<std::string>& v, int fileIndex) {
    std::string fileName = "file" + std::to_string(fileIndex) + ".txt";

    std::ofstream fileOutput(dirOutput_ / fileName);
    for (const auto& str : v) {
        fileOutput << str << std::endl;
    }

    mutex_.lock();
    --activeThread_;
    mutex_.unlock();
}

void MapReduce::processVector_(std::vector<char>& vec) {
    size_t erasePos = 0;

    auto itReverse = std::find(vec.rbegin(), vec.rend(), '\n');
    if (itReverse == vec.rend()) {
        auto it = std::find(vec.begin(), vec.end(), '\0');
        erasePos = std::distance(vec.begin(), it);
    } else {
        erasePos = std::distance(vec.begin(), itReverse.base());
    }

    if (erasePos > 0) {
        vec.erase(vec.begin() + erasePos, vec.end());
        chunks_.emplace_back(std::move(std::string(vec.begin(), vec.end())));

        std::fill(vec.begin(), vec.end(), '\0');
    }
}

std::unordered_map<std::string, unsigned int> MapReduce::getStringByMinSize_(const std::vector<std::string>& v) {
    std::unordered_map<std::string, unsigned int> m;

    for (const std::string& str : v) {
        std::string partOfString = str.substr(0, minPrefix_);

        auto it = m.find(partOfString);
        if (it != m.end()) {
            ++m[partOfString];
        } else {
            m[partOfString] = 1;
        }
    }

    return m;
}

bool MapReduce::prepareOutputDir_() {
    std::error_code e;

    bool dirExists = std::filesystem::exists(dirOutput_, e);
    if (e) {
        errorString_ = e.message();
        return false;
    }

    if (dirExists) {
        (void)std::filesystem::remove_all(dirOutput_, e);
        if (e) {
            errorString_ = e.message();
            return false;
        }
    }

    (void)std::filesystem::create_directory(dirOutput_, e);
    if (e) {
        errorString_ = e.message();
        return false;
    }

    return true;
}
