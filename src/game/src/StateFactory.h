#pragma once

#include "RendererPool.h"
#include "State.h"
#include "StateFactory.h"
#include "States.h"
#include "Window.h"
#include "editor/TileMap.h"

namespace game
{
class StateFactory
{
public:
    StateFactory(std::shared_ptr<window::Window>, std::shared_ptr<graphics::RendererPool>,
                 std::shared_ptr<utils::FileAccess>, States&, TileMap&);

    std::unique_ptr<State> createState(StateType);

private:
    const std::shared_ptr<window::Window> window;
    const std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<utils::FileAccess> fileAccess;
    States& states;
    TileMap& tileMap;
};
}