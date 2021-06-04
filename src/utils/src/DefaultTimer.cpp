#include "DefaultTimer.h"

namespace utils
{

void DefaultTimer::restart()
{
    clock.restart();
}

float DefaultTimer::getElapsedSeconds() const
{
    return clock.getElapsedTime().asSeconds();
}

}