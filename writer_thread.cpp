#include "writer_thread.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

WriterThread& WriterThread::instance() {
    static WriterThread writerThread;
    return writerThread;
}

bool WriterThread::isRun() const { return isRun_; }

void WriterThread::start(unsigned int threadCount) {
    if (isRun_) {
        return;
    }

    for (unsigned int i = 0; i < threadCount; ++i) {
        std::thread(&WriterThread::worker_, this).detach();
    }

    isRun_ = true;
}

void WriterThread::add(const FileLogger& logger) {
    if (isRun_) {
        queue_.push(std::move(logger));
        condition_.notify_one();
    }
}

void WriterThread::worker_() {
    std::ostringstream stream;
    stream << std::this_thread::get_id();

    std::unique_lock<std::mutex> lock(mutex_);

    while (true) {
        while (queue_.empty()) {
            condition_.wait(lock);
            if (isFinished_.load()) {
                return;
            }
        }

        auto* cmd = &queue_.front();

        cmd->addThreadIdentifier(stream.str());
        cmd->saveToFile();

        queue_.pop();
    }
}

WriterThread::~WriterThread() {
    isFinished_.store(true);
    condition_.notify_all();
}
