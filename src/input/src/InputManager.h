#pragma once

#include "Input.h"
#include "InputApi.h"

namespace input
{
class INPUT_API InputManager
{
public:
    virtual ~InputManager() = default;

    virtual const Input& readInput() = 0;
};
}