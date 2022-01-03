#include "Level1Controller.h"

#include <utility>

#include "MovementComponent.h"
#include "TimerFactory.h"

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
    mainCharacters.player = worldBuilder->getPlayer();

    for (const auto& worldObject : worldObjects)
    {
        ownersManager->add(worldObject);
    }

    ownersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();
}

SwitchToNextLevel Level1Controller::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    const auto elapsedSeconds = timer->getElapsedSeconds();

    if (elapsedSeconds > 0.3 and not playerBlocked)
    {
        auto playerMovementComponent =
            mainCharacters.player->getComponent<components::core::MovementComponent>();
        playerMovementComponent->lock();
        playerBlocked = true;
    }

    if (elapsedSeconds > 5 and not playerUnblocked)
    {
        auto playerMovementComponent =
            mainCharacters.player->getComponent<components::core::MovementComponent>();
        playerMovementComponent->unlock();
        playerUnblocked = true;
    }

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