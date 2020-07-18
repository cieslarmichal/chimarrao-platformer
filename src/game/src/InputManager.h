#pragma once
#include "ObservableInput.h"

namespace game
{
class InputManager : public ObservableInput
{
public:
    virtual ~InputManager() = default;

    virtual void readInput() = 0;
};
}