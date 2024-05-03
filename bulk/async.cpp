#include "async.h"

#include <thread>

#include "cmd_mgr.h"
#include "console_logger.h"
#include "file_logger.h"
#include "writer_thread.h"

struct AsyncHandle {
    AsyncHandle(unsigned int bulk) : cmd(bulk) {
        cmd.addObserver(&fileLogger_);
        cmd.addObserver(&consoleLogger_);
    }

    FileLogger fileLogger_{};
    ConsoleLogger consoleLogger_{};
    CommandMgr cmd;
};

inline static AsyncHandle* handleCast(async::handle_t handle) { return static_cast<AsyncHandle*>(handle); }

async::handle_t async::connect(unsigned int bulk) {
    if (!WriterThread::instance().isRun()) {
        auto threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) {
            return nullptr;
        }

        WriterThread::instance().start(threadCount);
    }

    return (new AsyncHandle(bulk));
}

void async::receive(handle_t handle, const char* data, size_t size) {
    handleCast(handle)->cmd.pushCmd(std::string(data, size));
}

void async::disconnect(handle_t handle) { delete static_cast<AsyncHandle*>(handle); }

void async::flush(handle_t handle) { /*handleCast(handle)->cmd.flush();*/ }
