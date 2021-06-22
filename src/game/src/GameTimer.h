#pragma once

#include "DeltaTime.h"

namespace game
{
class GameTimer
{
public:
    explicit GameTimer();

    utils::DeltaTime getDurationFromLastUpdate() const;

private:
    mutable utils::TimePoint timePointFromPreviousFunctionCall;
};
}
