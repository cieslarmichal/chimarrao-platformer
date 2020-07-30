#pragma once

#include <memory>

#include "Timer.h"
#include "Window.h"
#include "InputManager.h"
#include <stack>
#include <map>
#include "GameState.h"

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

    Timer timer;
    utils::DeltaTime dt;
    std::shared_ptr<graphics::Window> window;
    std::unique_ptr<InputManager> inputManager;
    std::stack<std::unique_ptr<GameState>> states;
};
}