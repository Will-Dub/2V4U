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

    void debug(const char* message) override;
    void info(const char* message) override;
    void error(const char* message) override;
    void warn(const char* message) override;
};
} // namespace App::Infrastructure