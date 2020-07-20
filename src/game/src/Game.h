#pragma once

#include <memory>

#include "Timer.h"
#include "Window.h"
#include "InputManager.h"

namespace game
{
class Game
{
public:
    explicit Game();

    void run();

private:
    void processInput();
    void update(DeltaTime);
    void render();

    Timer timer;
    std::unique_ptr<graphics::Window> window;
    std::unique_ptr<InputManager> inputManager;
};
}