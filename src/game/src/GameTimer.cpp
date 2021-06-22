#include "GameTimer.h"

namespace game
{

GameTimer::GameTimer()
{
    timePointFromPreviousFunctionCall = std::chrono::high_resolution_clock::now();
}

utils::DeltaTime GameTimer::getDurationFromLastUpdate() const
{
    auto now = std::chrono::high_resolution_clock::now();
    auto deltaTime = now - timePointFromPreviousFunctionCall;
    timePointFromPreviousFunctionCall = now;
    return deltaTime;
}
}