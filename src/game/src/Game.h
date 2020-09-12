#pragma once

#include <map>
#include <memory>
#include <stack>

#include "../../input/src/InputManager.h"
#include "GameState.h"
#include "PhysicsEngine.h"
#include "RendererPool.h"
#include "Timer.h"
#include "Window.h"

namespace game
{
class Game
{
public:
    explicit Game();

    void run();

private:
    void processInput();
    void update();
    void render();
    void initStates();

    utils::Timer timer;
    utils::DeltaTime dt;
    std::shared_ptr<graphics::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<physics::PhysicsEngine> physicsEngine;
    std::unique_ptr<input::InputManager> inputManager;
    std::stack<std::unique_ptr<GameState>> states;
};
}