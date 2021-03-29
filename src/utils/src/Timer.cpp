#include "Timer.h"

namespace utils
{
Timer::Timer() : elapsed{clock.getElapsedTime()}
{
}

void Timer::start()
{
    timeOfPreviousUpdate = std::chrono::high_resolution_clock::now();
}

void Timer::restart()
{
    clock.restart();
}

utils::DeltaTime Timer::getDurationFromLastUpdate()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto deltaTime = now - timeOfPreviousUpdate;
    timeOfPreviousUpdate = now;
    return deltaTime;
}

float Timer::getElapsedSeconds()
{
    elapsed = clock.getElapsedTime();
    return elapsed.asSeconds();
}

float Timer::getElapsedAndRestart()
{
    return clock.restart().asSeconds();
}

}