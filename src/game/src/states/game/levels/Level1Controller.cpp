#include "Level1Controller.h"

#include <utility>

#include "AnimationComponent.h"
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
    : worldBuilder{std::make_unique<Level1WorldBuilder>(characterFactory, obstacleFactory, sharedContext,
                                                        this)},
      ownersManager{std::move(ownersManagerInit)},
      fileAccess{std::move(fileAccessInit)},
      timeNeededToStartFirstDialogue{0.3f},
      sleepTime{5.f},
      playerSleeping{false}
{
    tileMap->loadFromFile(levelMap);

    const auto worldObjects = worldBuilder->buildWorldObjects(tileMap);
    mainCharacters.player = worldBuilder->getPlayer();
    mainCharacters.rabbit = worldBuilder->getRabbit();
    mainCharacters.npc = worldBuilder->getNpc();

    for (const auto& worldObject : worldObjects)
    {
        ownersManager->add(worldObject);
    }

    ownersManager->processNewObjects();

    startFirstDialogueTimer = utils::TimerFactory::createTimer();
    sleepTimer = utils::TimerFactory::createTimer();

    dialoguesController = std::make_unique<Level1DialoguesController>(
        &mainCharacters, std::make_unique<DefaultDialoguesReader>(fileAccess),
        utils::TimerFactory::createTimer());
}

SwitchToNextLevel Level1Controller::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (startFirstDialogueTimer->getElapsedSeconds() > timeNeededToStartFirstDialogue)
    {
        std::call_once(playerWithRabbitDialogueStarted,
                       [this]
                       {
                           mainCharacters.player->getComponent<components::core::AnimationComponent>()
                               ->setAnimationDirection(animations::AnimationDirection::Left);
                           dialoguesController->startPlayerWithRabbitDialogue();
                       });
    }

    if (playerSleeping and sleepTimer->getElapsedSeconds() > sleepTime)
    {
        playerSleeping = false;
        mainCharacters.player->getComponent<components::core::AnimationComponent>()->setAnimation(
            animations::AnimationType::Idle);
        mainCharacters.player->getComponent<components::core::MovementComponent>()->unlock();
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

void Level1Controller::campfireAction()
{
    sleepTimer->restart();
    playerSleeping = true;
    mainCharacters.player->getComponent<components::core::AnimationComponent>()->setAnimation(
        animations::AnimationType::Sleep);
    mainCharacters.player->getComponent<components::core::MovementComponent>()->lock();
}

void Level1Controller::druidAction()
{
    dialoguesController->startPlayerWithDruidDialogue();
}

}