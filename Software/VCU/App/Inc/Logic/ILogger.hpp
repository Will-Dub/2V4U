#pragma once

namespace App::Logic {

enum class LogLevel
{
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_NONE
};

class ILogger
{
  public:
    virtual ~ILogger() = default;

    virtual void setLogLevel(LogLevel level) = 0;

    virtual void debugf(const char* format, ...) = 0;
    virtual void infof(const char* format, ...) = 0;
    virtual void errorf(const char* format, ...) = 0;
    virtual void warnf(const char* format, ...) = 0;
};
} // namespace App::Logic