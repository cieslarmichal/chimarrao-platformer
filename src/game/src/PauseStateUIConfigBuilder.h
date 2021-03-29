#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class PauseState;

class PauseStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createPauseUIConfig(PauseState*);
};
}