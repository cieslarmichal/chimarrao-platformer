#pragma once

#include <stack>

#include "MusicManager.h"
#include "RendererPool.h"
#include "StateFactory.h"
#include "States.h"
#include "Window.h"
#include "editor/TileMap.h"

namespace game
{
class DefaultStates : public States
{
public:
    DefaultStates(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                  const std::shared_ptr<utils::FileAccess>&, std::shared_ptr<TileMap>,
                  std::shared_ptr<audio::MusicManager>);

    StatesStatus updateCurrentState(const utils::DeltaTime&, const input::Input&) override;
    void deactivateCurrentState() override;
    void addNextState(StateType) override;
    void render() override;

private:
    void backToThePreviousState();
    void backToTheMenuState();

    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<StateFactory> stateFactory;
    std::stack<std::unique_ptr<State>> states;
};
}