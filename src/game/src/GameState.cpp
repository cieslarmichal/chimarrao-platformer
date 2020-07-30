#include "GameState.h"

namespace game
{

GameState::GameState(std::shared_ptr<graphics::Window> windowInit, InputManager& inputManagerInit)
    : window{std::move(windowInit)}, inputManager{inputManagerInit}
{
}
}