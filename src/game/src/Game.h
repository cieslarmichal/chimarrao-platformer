#pragma once

#include <memory>

#include "InputManager.h"
#include "RendererPool.h"
#include "State.h"
#include "Timer.h"
#include "Window.h"
#include "States.h"
#include "editor/TileMap.h"

namespace game
{
class Game
{
public:
    explicit Game();

    void run();

private:
    void update();
    void render();

    utils::Timer timer;
    utils::DeltaTime deltaTime;
    std::shared_ptr<window::Window> window;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<input::InputManager> inputManager;
    std::unique_ptr<TileMap> tileMap;
    std::unique_ptr<States> states;
};
}