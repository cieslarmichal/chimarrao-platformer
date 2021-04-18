#pragma once

#include "RendererPool.h"
#include "StateFactory.h"
#include "States.h"
#include "Window.h"
#include "editor/TileMap.h"
#include "State.h"

namespace game
{
class StateFactory
{
public:
    StateFactory(std::shared_ptr<window::Window>, std::shared_ptr<graphics::RendererPool>, States&,
                 TileMap&);

    std::unique_ptr<State> createState(StateType);

private:
    const std::shared_ptr<window::Window> window;
    const std::shared_ptr<graphics::RendererPool> rendererPool;
    States& states;
    TileMap& tileMap;
};
}