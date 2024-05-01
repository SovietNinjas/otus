#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <string>

#include "file_logger.h"

class WriterThread {
   public:
    static WriterThread& instance();

    bool isRun() const;
    void start(unsigned int thread_count);
    void add(const FileLogger& logger);

   private:
    void worker_();

   private:
    WriterThread() = default;
    ~WriterThread();
    WriterThread(const WriterThread&) = delete;
    WriterThread(WriterThread&&) = delete;
    WriterThread& operator=(const WriterThread&) = delete;
    WriterThread& operator=(WriterThread&&) = delete;

   private:
    bool isRun_ = false;
    std::atomic<bool> isFinished_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<FileLogger> queue_;
};
