#pragma once

#include <chrono>

#include "DeltaTime.h"

namespace utils
{
class Timer
{
private:
    utils::TimePoint startTimePoint;
    utils::TimePoint previousUpdateTimePoint;

public:
    void start();
    utils::DeltaTime getDurationFromLastUpdate();
};
}
