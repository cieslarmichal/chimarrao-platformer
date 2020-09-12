#include "GameState.h"

namespace game
{

GameState::GameState(std::shared_ptr<graphics::Window> windowInit, input::InputManager& inputManagerInit,
                     std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                     std::shared_ptr<physics::PhysicsEngine> physicsEngineInit)
    : window{std::move(windowInit)},
      inputManager{inputManagerInit},
      rendererPool{std::move(rendererPoolInit)},
      physicsEngine{std::move(physicsEngineInit)}
{
}
}