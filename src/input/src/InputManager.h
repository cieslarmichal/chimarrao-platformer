#pragma once

#include "ObservableInput.h"

namespace input
{
class InputManager : public ObservableInput
{
public:
    virtual ~InputManager() = default;

    virtual void readInput() = 0;
};
}