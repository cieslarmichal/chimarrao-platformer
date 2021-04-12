#pragma once

#include "InputApi.h"
#include "ObservableInput.h"

namespace input
{
class INPUT_API InputManager : public ObservableInput
{
public:
    virtual ~InputManager() = default;

    virtual void readInput() = 0;
};
}