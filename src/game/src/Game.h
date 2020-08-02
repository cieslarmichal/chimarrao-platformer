#pragma once

#include <memory>

#include "Timer.h"
#include "Window.h"
#include "InputManager.h"
#include <stack>
#include <map>
#include "GameState.h"
#include "RendererPool.h"

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
    std::unique_ptr<InputManager> inputManager;
    std::stack<std::unique_ptr<GameState>> states;
};
}