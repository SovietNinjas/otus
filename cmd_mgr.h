#pragma once

#include <chrono>
#include <fstream>

class FileLogger {
   public:
    FileLogger& operator<<(std::string_view sv) {
        ss_ << sv;
        return *this;
    }

    inline void print() const {
        std::cout << ss_.str();
        saveToFile(ss_.str());
    }

    inline void createNew(const std::string& fileName) {
        ss_.str("");
        fileName_ = fileName + getCurrentTime_();
    }

   private:
    std::string getCurrentTime_() const {
        using namespace std::chrono;
        auto time = system_clock::to_time_t(system_clock::now());
        return std::to_string(time);
    }

    inline void saveToFile(std::string_view sv) const {
        std::ofstream ofs(fileName_);
        if (ofs.is_open()) {
            ofs << sv;
            ofs.close();
        }
    }

    std::string fileName_;
    std::stringstream ss_;
};

class CommandMgr {
   public:
    explicit CommandMgr(std::size_t maxSize) : maxSize_(maxSize) {}
    ~CommandMgr() {
        if (nestLevel_ == 0) endBlockBehavior_();
    }

    void pushCmd(std::string_view cmd) {
        if (cmd == "{") {
            if (++nestLevel_ == 1) {
                endBlockBehavior_();
            }
        } else if (cmd == "}") {
            if (--nestLevel_ == 0) {
                endBlockBehavior_();
            }
        } else {
            saveCmd_(cmd);

            if (nestLevel_ == 0 && cmdCounter_ == maxSize_) {
                endBlockBehavior_();
            }
        }
    }

   private:
    inline void endBlockBehavior_() {
        if (cmdCounter_ != 0) {
            cmdCounter_ = 0;
            logger_ << "\n";
            logger_.print();
        }
    }

    inline void saveCmd_(std::string_view cmd) {
        if (cmdCounter_ == 0) {
            resetLogger_();
            logger_ << "bulk: " << cmd;
        } else {
            logger_ << ", " << cmd;
        }
        ++cmdCounter_;
    }

    inline void resetLogger_() { logger_.createNew("bulk"); }

    std::size_t nestLevel_ = 0, maxSize_ = 0, cmdCounter_ = 0;
    FileLogger logger_;
};
