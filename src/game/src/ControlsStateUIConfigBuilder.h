#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class ControlsState;

class ControlsStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createControlsUIConfig(ControlsState*);
};
}