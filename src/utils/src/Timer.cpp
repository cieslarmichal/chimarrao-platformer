#include "Timer.h"

namespace utils
{
utils::DeltaTime Timer::getDurationFromLastUpdate()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto dt = now - previousUpdateTimePoint;
    previousUpdateTimePoint = now;
    return dt;
}

void Timer::start()
{
    previousUpdateTimePoint = std::chrono::high_resolution_clock::now();
}
}