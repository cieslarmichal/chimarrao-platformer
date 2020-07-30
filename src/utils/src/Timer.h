#pragma once

#include <chrono>
#include "DeltaTime.h"

namespace utils
{
class Timer
{
private:
    TimePoint startTimePoint;
    TimePoint previousUpdateTimePoint;
public:
    void start();
    DeltaTime getDurationFromLastUpdate();
};
}
