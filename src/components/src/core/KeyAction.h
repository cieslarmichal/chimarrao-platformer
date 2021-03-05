#pragma once
#include "InputKey.h"
#include "functional"

namespace components::core
{
struct KeyAction
{
    input::InputKey key;
    std::function<void(void)> action;
    bool clicked{false};
};
}