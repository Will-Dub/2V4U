#include "StmLogger.hpp"

namespace App::Infrastructure {
StmLogger::StmLogger() {}

void StmLogger::debug(const char* message)
{
    printf(LOG_COLOR_CYAN "[%8lu][DEBUG] %s" LOG_COLOR_RESET "\r\n", HAL_GetTick(), message);
}

void StmLogger::info(const char* message)
{
    printf(LOG_COLOR_GREEN "[%8lu][INFO] %s" LOG_COLOR_RESET "\r\n", HAL_GetTick(), message);
}

void StmLogger::error(const char* message)
{
    printf(LOG_COLOR_RED "[%8lu][ERROR] %s" LOG_COLOR_RESET "\r\n", HAL_GetTick(), message);
}
void StmLogger::warn(const char* message)
{
    printf(LOG_COLOR_YELLOW "[%8lu][WARN] %s" LOG_COLOR_RESET "\r\n", HAL_GetTick(), message);
}
} // namespace App::Infrastructure