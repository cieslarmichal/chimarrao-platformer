#include "GameState.h"

namespace game
{

GameState::GameState(std::shared_ptr<gui::Window> windowInit,
                     std::shared_ptr<input::InputManager> inputManagerInit,
                     std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : window{std::move(windowInit)},
      inputManager{std::move(inputManagerInit)},
      rendererPool{std::move(rendererPoolInit)}
{
}
}