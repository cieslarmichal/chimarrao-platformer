#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class MenuState;

class MenuStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createMenuUIConfig(MenuState*);
};
}