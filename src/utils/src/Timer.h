#pragma once

#include <chrono>
#include <SFML/System/Clock.hpp>

#include "DeltaTime.h"

namespace utils
{
class Timer
{
public:
    Timer();

    void start();
    void restart();
    utils::DeltaTime getDurationFromLastUpdate();
    float getElapsedSeconds();
    float getElapsedAndRestart();

private:
    utils::TimePoint timeOfPreviousUpdate;
    sf::Clock clock;
    sf::Time elapsed;
};
}
