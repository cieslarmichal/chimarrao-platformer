#pragma once

#include "InputStatus.h"

namespace input
{
class InputObserver
{
public:
    virtual ~InputObserver() = default;

    virtual void handleInputStatus(const InputStatus&) = 0;
};
}