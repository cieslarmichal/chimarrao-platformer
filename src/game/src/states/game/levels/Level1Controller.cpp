#include "Level1Controller.h"

#include <utility>

namespace game
{
namespace
{
const std::string mapsDirectory{utils::ProjectPathReader::getProjectRootPath() + "maps/story/"};
const std::string levelMap{mapsDirectory + "level1.map"};
}

Level1Controller::Level1Controller(const std::shared_ptr<TileMap>& tileMap,
                                   std::unique_ptr<ComponentOwnersManager> ownersManagerInit,
                                   std::shared_ptr<WorldBuilder> worldBuilderInit)
    : ownersManager{std::move(ownersManagerInit)}, worldBuilder{std::move(worldBuilderInit)}
{
    tileMap->loadFromFile(levelMap);

    const auto worldObjects = worldBuilder->buildWorldObjects(tileMap);

    for (const auto& worldObject : worldObjects)
    {
        ownersManager->add(worldObject);
    }

    ownersManager->processNewObjects();
}

SwitchToNextLevel Level1Controller::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    ownersManager->update(deltaTime, input);
    ownersManager->processRemovals();

    return false;
}

void Level1Controller::activate()
{
    ownersManager->activate();
}

void Level1Controller::deactivate()
{
    ownersManager->deactivate();
}
}