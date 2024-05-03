#pragma once
#include <fstream>
#include <sstream>

#include "logger_observer.h"
#include "utils.h"

class FileLogger : public LoggerObserver {
   public:
    inline void addThreadIdentifier(std::string_view sv) {
        fileName_ += '_';
        fileName_ += sv;
    }

    inline void saveToFile() {
        std::ofstream ofs(fileName_);
        if (ofs.is_open()) {
            ofs << oss_.str();
            ofs.close();
        }
    }

    FileLogger() = default;
    FileLogger(const FileLogger& other) {
        if (this != &other) {
            fileName_ = other.fileName_;
            oss_ << other.oss_.str();
        }
    }

   private:
    inline void write(std::string_view sv) override { oss_ << sv; }

    inline void createNew(std::string_view name) override {
        oss_.str("");
        fileName_ = name;
        fileName_ += utils::getCurrentTime();
    }

    void release() override;

    std::ostringstream oss_;
    std::string fileName_;
};
