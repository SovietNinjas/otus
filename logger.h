#pragma once

#include <algorithm>

#include "logger_observer.h"

class ConsoleLogger : public LoggerObserver {
   public:
    inline void createNew([[maybe_unused]] std::string_view name) override { oss_.str(""); }

    inline void write(std::string_view sv) override { oss_ << sv; }

    inline void release() override { std::cout << oss_.str(); }

   private:
    std::ostringstream oss_;
};

class FileLogger : public LoggerObserver {
    inline void write(std::string_view sv) override { oss_ << sv; }

    inline void createNew(std::string_view name) override {
        oss_.str("");
        fileName_ = name;
        fileName_ += getCurrentTime_();
    }

    inline void release() override {
        std::ofstream ofs(fileName_);
        if (ofs.is_open()) {
            ofs << oss_.str();
            ofs.close();
        }
    }

   private:
    inline std::string getCurrentTime_() const {
        using namespace std::chrono;
        auto time = system_clock::to_time_t(system_clock::now());
        return std::to_string(time);
    }

    std::ostringstream oss_;
    std::string fileName_;
};
