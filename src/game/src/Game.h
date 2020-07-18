#pragma once

#include <memory>

#include "Timer.h"
#include "Window.h"

namespace game
{
class Game
{
private:
    Timer timer;
    std::unique_ptr<graphics::Window> window;

    void processInput();

    void update(DeltaTime dt);

    void render();

public:
    Game();

    void run();
};
}