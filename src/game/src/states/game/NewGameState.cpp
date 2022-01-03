#include "NewGameState.h"

namespace game
{
namespace
{
const std::string mapsDirectory{utils::ProjectPathReader::getProjectRootPath() + "maps/story/"};
}

NewGameState::NewGameState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           const std::shared_ptr<TileMap>& tileMap)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit}
{
    const auto firstMap = mapsDirectory + "level1.map";
    tileMap->loadFromFile(firstMap);
}

NextState NewGameState::update(const utils::DeltaTime&, const input::Input&)
{
    states.deactivateCurrentState();
    states.addNextState(StateType::Game);
    return NextState::Same;
}

void NewGameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void NewGameState::render() {}

StateType NewGameState::getType() const
{
    return StateType::NewGame;
}

void NewGameState::activate() {}

void NewGameState::deactivate() {}

}