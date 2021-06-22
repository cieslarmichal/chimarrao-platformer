#include "TimerFactory.h"

#include "DefaultTimer.h"

namespace utils
{

std::unique_ptr<Timer> TimerFactory::createTimer()
{
    return std::make_unique<DefaultTimer>();
}
}