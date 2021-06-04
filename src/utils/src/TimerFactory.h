#pragma once

#include "Timer.h"
#include <memory>

namespace utils
{
class TimerFactory
{
public:
    static std::unique_ptr<Timer> createTimer();
};
}