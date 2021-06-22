#pragma once

namespace utils
{
class Timer
{
public:
    virtual ~Timer() = default;

    virtual void restart() = 0;
    virtual float getElapsedSeconds() const = 0;
};
}
