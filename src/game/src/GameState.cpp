#include "GameState.h"

namespace game
{

GameState::GameState(std::shared_ptr<graphics::Window> windowInit, InputManager& inputManagerInit,
                     std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : window{std::move(windowInit)}, inputManager{inputManagerInit}, rendererPool{std::move(rendererPoolInit)}
{
}
}