#pragma once

#include <string_view>

class LoggerObserver {
   public:
    virtual ~LoggerObserver() = default;

    virtual void createNew(std::string_view name) = 0;
    virtual void write(std::string_view sv) = 0;
    virtual void release() = 0;
};

class LoggerObservable {
   public:
    virtual ~LoggerObservable() = default;

    virtual void addObserver(LoggerObserver* observer) = 0;
};
