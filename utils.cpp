#include "utils.h"

utils::TimePoint utils::getTick() { return std::chrono::steady_clock::now(); }

uint64_t utils::getTickDiff(const TimePoint& t) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(getTick() - t).count();
}

std::string utils::getCurrentThreadID() {
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    return stream.str();
}
