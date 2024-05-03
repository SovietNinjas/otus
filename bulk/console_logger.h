#pragma once

#include <algorithm>
#include <future>
#include <iostream>

#include "logger_observer.h"
#include "writer_thread.h"

class ConsoleLogger : public LoggerObserver {
    inline void createNew([[maybe_unused]] std::string_view name) override { oss_.str(""); }

    inline void write(std::string_view sv) override { oss_ << sv; }

    inline void release() override {
        static auto print = [](const std::string& s) { std::cout << s; };
        (void)std::async(std::launch::async, std::ref(print), oss_.str());
    }

   private:
    std::ostringstream oss_;
};
