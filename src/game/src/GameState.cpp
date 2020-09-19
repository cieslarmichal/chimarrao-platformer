#include "GameState.h"

namespace game
{

GameState::GameState(std::shared_ptr<graphics::Window> windowInit,
                     std::shared_ptr<input::InputManager> inputManagerInit,
                     std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                     std::shared_ptr<physics::PhysicsEngine> physicsEngineInit)
    : window{std::move(windowInit)},
      inputManager{std::move(inputManagerInit)},
      rendererPool{std::move(rendererPoolInit)},
      physicsEngine{std::move(physicsEngineInit)}
{
}
}