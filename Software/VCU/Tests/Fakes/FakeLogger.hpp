#pragma once
#include "../../App/Inc/Logic/ILogger.hpp"

struct FakeLogger : App::Logic::ILogger
{
    void setLogLevel(App::Logic::LogLevel) override {}
    void debugf(const char*, ...) override {}
    void infof(const char*, ...) override {}
    void errorf(const char*, ...) override {}
    void warnf(const char*, ...) override {}
};