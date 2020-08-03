#pragma once

#include <memory>
#include <vector>

#include "DeltaTime.h"
#include "InputManager.h"
#include "PhysicsEngine.h"
#include "RendererPool.h"
#include "Window.h"

namespace game
{
class GameState
{
public:
    explicit GameState(std::shared_ptr<graphics::Window>, InputManager&,
                       std::shared_ptr<graphics::RendererPool>, std::shared_ptr<physics::PhysicsEngine>);
    virtual ~GameState() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void render() = 0;
    virtual void checkIfEnded() = 0;

protected:
    InputManager& inputManager;
    std::shared_ptr<graphics::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<physics::PhysicsEngine> physicsEngine;

private:
    bool stateIsEnded;
};
}