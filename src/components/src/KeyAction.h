#pragma once
#include "InputKey.h"
#include "functional"

struct KeyAction
{
    input::InputKey key;
    std::function<void(void)> action;
    bool clicked{false};
};
