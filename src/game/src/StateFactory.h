#pragma once

#include "PhysicsFactory.h"
#include "RendererPool.h"
#include "State.h"
#include "States.h"
#include "Window.h"
#include "editor/TileMap.h"
#include "MusicManager.h"

namespace game
{

class StateFactory
{
public:
    StateFactory(std::shared_ptr<window::Window>, std::shared_ptr<graphics::RendererPool>,
                 std::shared_ptr<utils::FileAccess>, States&, std::shared_ptr<TileMap>, std::shared_ptr<audio::MusicManager>);

    std::unique_ptr<State> createState(StateType);

private:
    const std::shared_ptr<window::Window> window;
    const std::shared_ptr<graphics::RendererPool> rendererPool;
    std::shared_ptr<utils::FileAccess> fileAccess;
    States& states;
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<physics::PhysicsFactory> collisionSystemFactory;
    std::shared_ptr<components::core::SharedContext> sharedContext;
    std::shared_ptr<audio::MusicManager> musicManager;
};
}