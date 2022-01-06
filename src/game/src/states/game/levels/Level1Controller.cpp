#include "Level1Controller.h"

#include <utility>

#include "DefaultDialoguesReader.h"
#include "Level1WorldBuilder.h"
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
                                   const std::shared_ptr<CharacterFactory>& characterFactory,
                                   const std::shared_ptr<ObstacleFactory>& obstacleFactory,
                                   const std::shared_ptr<components::core::SharedContext>& sharedContext,
                                   std::shared_ptr<utils::FileAccess> fileAccessInit)
    : ownersManager{std::move(ownersManagerInit)}, fileAccess{std::move(fileAccessInit)}
{
    tileMap->loadFromFile(levelMap);

    auto worldBuilder =
        std::make_unique<Level1WorldBuilder>(characterFactory, obstacleFactory, sharedContext, this);
    const auto worldObjects = worldBuilder->buildWorldObjects(tileMap);
    mainCharacters.player = worldBuilder->getPlayer();
    mainCharacters.rabbit = worldBuilder->getRabbit();
    mainCharacters.npc = worldBuilder->getNpc();

    for (const auto& worldObject : worldObjects)
    {
        ownersManager->add(worldObject);
    }

    ownersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();

    dialoguesController = std::make_unique<Level1DialoguesController>(
        &mainCharacters, std::make_unique<DefaultDialoguesReader>(fileAccess),
        utils::TimerFactory::createTimer());
}

SwitchToNextLevel Level1Controller::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    const auto elapsedSeconds = timer->getElapsedSeconds();

    if (elapsedSeconds > 0.3)
    {
        std::call_once(playerWithRabbitDialogueStarted,
                       [this] { dialoguesController->startPlayerWithRabbitDialogue(); });
    }

    dialoguesController->update();
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

Level1MainCharacters Level1Controller::getCharacters() const
{
    return mainCharacters;
}

}