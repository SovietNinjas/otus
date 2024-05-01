#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#include "logger_observer.h"
#include "utils.h"

class CommandMgr : public LoggerObservable {
   public:
    explicit CommandMgr(std::size_t maxSize) : maxSize_(maxSize) {}
    ~CommandMgr() {
        if (nestLevel_ == 0) endBlockBehavior_();
    }

    void pushCmd(std::string_view cmd) {
        auto v = utils::splitString(cmd, '\n');

        for (auto sv : v) {
            if (sv == "{") {
                if (++nestLevel_ == 1) {
                    endBlockBehavior_();
                }
            } else if (sv == "}") {
                if (--nestLevel_ == 0) {
                    endBlockBehavior_();
                }
            } else {
                saveCmd_(sv);

                if (nestLevel_ == 0 && cmdCounter_ == maxSize_) {
                    endBlockBehavior_();
                }
            }
        }
    }

    void addObserver(LoggerObserver* observer) override { observersList_.push_back(observer); }

   private:
    inline void endBlockBehavior_() {
        if (cmdCounter_ != 0) {
            cmdCounter_ = 0;
            loggerWrite_("\n");
            loggerRelease_();
        }
    }

    inline void saveCmd_(std::string_view cmd) {
        std::ostringstream oss;

        if (cmdCounter_ == 0) {
            loggerCreateNew_();
            oss << "bulk: " << cmd;
        } else {
            oss << ", " << cmd;
        }

        loggerWrite_(oss.str());

        ++cmdCounter_;
    }

    inline void loggerWrite_(std::string_view sv) {
        std::ranges::for_each(observersList_, [sv](auto logger) { logger->write(sv); });
    }

    inline void loggerRelease_() {
        std::ranges::for_each(observersList_, [](auto logger) { logger->release(); });
    }

    inline void loggerCreateNew_() {
        std::ranges::for_each(observersList_, [](auto logger) { logger->createNew("bulk"); });
    }

    std::vector<LoggerObserver*> observersList_{};
    std::size_t nestLevel_ = 0, maxSize_ = 0, cmdCounter_ = 0;
};
