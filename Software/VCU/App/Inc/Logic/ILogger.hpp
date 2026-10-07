#pragma once

namespace App::Logic {
class ILogger
{
  public:
    virtual ~ILogger() = default;

    virtual void debug(const char* message) = 0;
    virtual void info(const char* message) = 0;
    virtual void error(const char* message) = 0;
    virtual void warn(const char* message) = 0;
};
} // namespace App::Logic