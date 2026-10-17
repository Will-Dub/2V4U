#pragma once

#include "ILogger.hpp"
#include "main.h"

#include <stdio.h>

#define LOG_COLOR_RESET "\x1b[0m"
#define LOG_COLOR_RED "\x1b[31m"
#define LOG_COLOR_GREEN "\x1b[32m"
#define LOG_COLOR_YELLOW "\x1b[33m"
#define LOG_COLOR_CYAN "\x1b[36m"

namespace App::Infrastructure {

class StmLogger : public App::Logic::ILogger
{
  public:
    StmLogger();

    void setLogLevel(App::Logic::LogLevel level) override;
    void debugf(const char* format, ...);
    void infof(const char* format, ...);
    void warnf(const char* format, ...);
    void errorf(const char* format, ...);

  private:
    App::Logic::LogLevel m_currentLogLevel = App::Logic::LogLevel::LEVEL_INFO;

    void logRaw(const char* message);
    void processLog(const char* color, const char* levelStr, const char* format, va_list args);
};
} // namespace App::Infrastructure