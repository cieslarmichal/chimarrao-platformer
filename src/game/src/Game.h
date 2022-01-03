#pragma once

#include <memory>

#include "GameTimer.h"
#include "InputManager.h"
#include "RendererPool.h"
#include "State.h"
#include "States.h"
#include "TileMap.h"
#include "Window.h"

namespace game
{
class Game
{
public:
    explicit Game(std::shared_ptr<window::Window> window, std::shared_ptr<input::InputManager> inputManager,
                  std::unique_ptr<States> states);

    void run();

private:
    StatesStatus update();
    void render();

    GameTimer timer;
    std::shared_ptr<window::Window> window;
    std::shared_ptr<input::InputManager> inputManager;
    std::unique_ptr<States> states;
};
}