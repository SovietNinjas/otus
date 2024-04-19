#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class Differ {
   public:
    Differ(const std::vector<std::string>& dirPathList, const std::vector<std::string>& excludeDirList,
           const std::vector<std::string>& maskList, bool is_recursive, unsigned int minSize,
           uint64_t chunkSize);

    const std::string& getErrorString() const;

    bool init();

    bool run();

   private:
    bool diff_(std::ifstream& file, const std::string& file_path_right, uint64_t size);

    void insertToMap_(const std::string& file_left, const std::string& file_right);

    void printResult_();

   private:
    std::string errorString_;
    std::vector<std::string> pathDirList_;
    std::vector<std::string> excludeDirList_;
    std::vector<std::string> maskList_;
    bool isRecursive_;
    unsigned int minSize_;
    uint64_t chunkSize_;
    std::map<std::string, std::vector<std::string>> mapSimilar_;
};
