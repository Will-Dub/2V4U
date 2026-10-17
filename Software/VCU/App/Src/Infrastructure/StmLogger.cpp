#include "StmLogger.hpp"

#include "main.h"

#include <stdarg.h>
#include <stdio.h>

namespace App::Infrastructure {

StmLogger::StmLogger() : m_currentLogLevel(App::Logic::LogLevel::LEVEL_INFO) {}

void StmLogger::setLogLevel(App::Logic::LogLevel level)
{
    m_currentLogLevel = level;
}

void StmLogger::processLog(const char* color, const char* levelStr, const char* format,
                           va_list args)
{
    char buffer[128];
    vsnprintf(buffer, sizeof(buffer), format, args);
    printf("%s[%8lu][%s] %s%s\r\n", color, HAL_GetTick(), levelStr, buffer, LOG_COLOR_RESET);
}

void StmLogger::debugf(const char* format, ...)
{
    if (m_currentLogLevel > App::Logic::LogLevel::LEVEL_DEBUG)
        return;
    va_list args;
    va_start(args, format);
    processLog(LOG_COLOR_CYAN, "DEBUG", format, args);
    va_end(args);
}

void StmLogger::infof(const char* format, ...)
{
    if (m_currentLogLevel > App::Logic::LogLevel::LEVEL_INFO)
        return;
    va_list args;
    va_start(args, format);
    processLog(LOG_COLOR_GREEN, "INFO", format, args);
    va_end(args);
}

void StmLogger::warnf(const char* format, ...)
{
    if (m_currentLogLevel > App::Logic::LogLevel::LEVEL_WARN)
        return;
    va_list args;
    va_start(args, format);
    processLog(LOG_COLOR_YELLOW, "WARN", format, args);
    va_end(args);
}

void StmLogger::errorf(const char* format, ...)
{
    if (m_currentLogLevel > App::Logic::LogLevel::LEVEL_ERROR)
        return;
    va_list args;
    va_start(args, format);
    processLog(LOG_COLOR_RED, "ERROR", format, args);
    va_end(args);
}

} // namespace App::Infrastructure