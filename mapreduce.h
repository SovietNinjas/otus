#pragma once

#include <filesystem>
#include <mutex>
#include <unordered_map>
#include <vector>

class MapReduce {
   public:
    using ValueT = std::unordered_map<std::string, unsigned int>;

    MapReduce(unsigned int m, unsigned int r, const std::string& filePath);

    const std::string& getErrorString() const;
    size_t getMinPrefix_() const;

    bool map();
    void shuffle();
    bool reduce();

   private:
    bool split_();
    void workerMap_(std::string& s);
    void workerReduce_(const std::vector<std::string>& v, int fileIndex);
    void processVector_(std::vector<char>& vec);
    ValueT getStringByMinSize_(const std::vector<std::string>& v);
    bool prepareOutputDir_();

   private:
    std::string errorString_;
    unsigned int map_;
    unsigned int reduce_;
    std::string filePath_;
    std::filesystem::path dirOutput_;
    std::vector<std::string> chunks_;
    unsigned int activeThread_;
    std::mutex mutex_;
    std::vector<ValueT> vectorTotal_;
    size_t minPrefix_;
};
