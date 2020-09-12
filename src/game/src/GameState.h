#pragma once

#include <memory>
#include <vector>

#include "../../input/src/InputManager.h"
#include "DeltaTime.h"
#include "PhysicsEngine.h"
#include "RendererPool.h"
#include "Window.h"

namespace game
{
class GameState
{
public:
    explicit GameState(std::shared_ptr<graphics::Window>, input::InputManager&,
                       std::shared_ptr<graphics::RendererPool>, std::shared_ptr<physics::PhysicsEngine>);
    virtual ~GameState() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual void checkIfEnded() = 0;

protected:
    input::InputManager& inputManager;
    std::shared_ptr<graphics::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<physics::PhysicsEngine> physicsEngine;

private:
    bool stateIsEnded;
};
}