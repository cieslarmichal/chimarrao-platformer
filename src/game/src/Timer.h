#pragma once

#include <chrono>
#include "DeltaTime.h"

namespace game
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
