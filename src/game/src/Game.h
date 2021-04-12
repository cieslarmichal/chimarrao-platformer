#pragma once

#include <map>
#include <memory>
#include <stack>

#include "InputManager.h"
#include "RendererPool.h"
#include "State.h"
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
    void lateUpdate();
    void render();
    void initStates();
    void backToThePreviousState();
    void backToTheMenuState();

    utils::Timer timer;
    utils::DeltaTime dt;
    std::shared_ptr<window::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<input::InputManager> inputManager;
    std::stack<std::unique_ptr<State>> states;
};
}