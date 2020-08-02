#pragma once

#include "InputStatus.h"

namespace game
{
class InputObserver
{
public:
    virtual ~InputObserver() = default;

    virtual void handleInputStatus(const InputStatus&) = 0;
};
}