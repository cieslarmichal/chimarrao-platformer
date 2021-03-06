#pragma once

namespace components::ui
{
struct MouseOverActions
{
    std::function<void(void)> mouseOverAction;
    std::function<void(void)> mouseOutAction;
};
}