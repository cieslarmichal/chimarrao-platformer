#pragma once

#include <memory>

#include "Timer.h"

namespace utils
{
class TimerFactory
{
public:
    static std::unique_ptr<Timer> createTimer();
};
}