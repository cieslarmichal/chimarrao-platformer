#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class GameState;

class GameStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createGameUIConfig(GameState*);
};
}