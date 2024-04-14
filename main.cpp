#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

class FileLogger {
   public:
    explicit FileLogger(const std::string& name) : ofs_(name + getCurrentTime_()) {}

    FileLogger& operator<<(std::string_view sv) {
        if (ofs_.is_open()) {
            ofs_ << sv;
        }
        return *this;
    }

    inline bool empty() const { return ofs_.rdbuf()->in_avail() == 0; }

    inline void print() const { std::cout << ofs_.rdbuf(); }

    void close() { ofs_.close(); }

   private:
    std::string getCurrentTime_() const {
        using namespace std::chrono;
        auto time = system_clock::to_time_t(system_clock::now());
        return std::to_string(time);
    }

    std::ofstream ofs_{};
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

            if (nestLevel_ == 0 && cmdCount_ == maxSize_) {
                endBlockBehavior_();
            }
        }
    }

   private:
    inline void endBlockBehavior_() {
        if (!logger_.empty()) {
            logger_ << "\n";
            logger_.print();
            logger_.close();
            cmdCount_ = 0;
        }
    }

    inline void saveCmd_(std::string_view cmd) {
        if (logger_.empty()) {
            logger_ << "bulk: " << cmd;
            createNewFile();
        } else {
            logger_ << ", " << cmd;
        }
        ++cmdCount_;
    }

    inline void createNewFile() { logger_ = FileLogger("bulk"); }

    std::size_t nestLevel_ = 0, cmdCount_ = 0, maxSize_ = 0;
    FileLogger logger_{""};
};

int main(int, char**) {
    std::size_t querryCount = 0;

    std::cin >> querryCount;

    CommandMgr cmdMgr(querryCount);

    for (std::string cmd; std::cin >> cmd;) {
        cmdMgr.pushCmd(std::move(cmd));
    }

    return 0;
}
