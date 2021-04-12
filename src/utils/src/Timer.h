#pragma once

#include <SFML/System/Clock.hpp>
#include <chrono>

#include "DeltaTime.h"
#include "UtilsApi.h"

namespace utils
{
class UTILS_API Timer
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
