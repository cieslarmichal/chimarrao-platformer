#pragma once

#include "KeyboardStatus.h"

namespace game
{
class InputObserver
{
public:
    virtual ~InputObserver() = default;

    virtual void update(const KeyboardStatus&) = 0;
};
}