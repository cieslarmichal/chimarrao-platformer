#pragma once

#include "gmock/gmock.h"

#include "Timer.h"

namespace utils
{
class TimerMock : public Timer
{
public:
    MOCK_METHOD(void, restart, (), (override));
    MOCK_METHOD(float, getElapsedSeconds, (), (const override));
};
}