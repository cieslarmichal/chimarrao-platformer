#pragma once

#include <SFML/System/Clock.hpp>

#include "Timer.h"

namespace utils
{
class DefaultTimer : public Timer
{
public:
    void restart() override;
    float getElapsedSeconds() const override;

private:
    sf::Clock clock;
};
}
