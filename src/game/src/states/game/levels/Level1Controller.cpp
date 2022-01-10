#include "Level1Controller.h"

#include <utility>

#include "AnimationComponent.h"
#include "DefaultDialoguesReader.h"
#include "DialogueTextComponent.h"
#include "Level1WorldBuilder.h"
#include "LimitedSpaceActionComponent.h"
#include "MovementComponent.h"
#include "StoryGameState.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const std::string projectPath{utils::ProjectPathReader::getProjectRootPath()};
const std::string mapsDirectory{projectPath + "maps/story/"};
const std::string levelMap{mapsDirectory + "level1.map"};
const std::string gameBackgroundUIName{"gameBackground"};
const std::string gameBackgroundPathBlackAndWhite{projectPath +
                                                  "resources/BG/background_mountains_black_and_white.png"};
}

Level1Controller::Level1Controller(
    const std::shared_ptr<TileMap>& tileMap,
    std::shared_ptr<components::core::ComponentOwnersManager> ownersManagerInit,
    const std::shared_ptr<CharacterFactory>& characterFactory,
    const std::shared_ptr<ObstacleFactory>& obstacleFactory, const std::shared_ptr<ItemFactory>& itemFactory,
    const std::shared_ptr<components::core::SharedContext>& sharedContext,
    std::shared_ptr<utils::FileAccess> fileAccessInit,
    std::shared_ptr<components::ui::UIManager> uiManagerInit, StoryGameState* storyGameStateInit)
    : ownersManager{std::move(ownersManagerInit)},
      worldBuilder{std::make_unique<Level1WorldBuilder>(characterFactory, obstacleFactory, itemFactory,
                                                        sharedContext, this, ownersManager)},
      fileAccess{std::move(fileAccessInit)},
      uiManager{std::move(uiManagerInit)},
      storyGameState{storyGameStateInit},
      timeNeededToStartFirstDialogue{0.3f},
      sleepTime{5.f},
      deadTime{2.f},
      playerSleeping{false},
      playerDead{false},
      playerSleepingNextToLastCampfire{false},
      levelFinished{false},
      playerTalkedToDruid{false}
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
    deadTimer = utils::TimerFactory::createTimer();

    dialoguesController = std::make_unique<Level1DialoguesController>(
        &mainCharacters, std::make_unique<DefaultDialoguesReader>(fileAccess),
        utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer());
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
                           dialoguesController->startPlayerWithRabbitFirstDialogue();
                       });
    }

    if (playerSleeping and sleepTimer->getElapsedSeconds() > sleepTime)
    {
        playerSleeping = false;
        mainCharacters.player->getComponent<components::core::AnimationComponent>()->setAnimation(
            animations::AnimationType::Idle);
        mainCharacters.player->getComponent<components::core::MovementComponent>()->unlock();

        if (playerSleepingNextToLastCampfire)
        {
            playerSleepingNextToLastCampfire = false;
            mainCharacters.player->getComponent<components::core::AnimationComponent>()
                ->setAnimationDirection(animations::AnimationDirection::Left);
            dialoguesController->startPlayerWithRabbitLastDialogue();
            levelFinished = true;
        }
    }

    if (levelFinished)
    {
        std::call_once(backgroundChanged,
                       [this]
                       {
                           uiManager->setTexture(gameBackgroundUIName, gameBackgroundPathBlackAndWhite);
                           uiManager->setColor(gameBackgroundUIName, graphics::Color(254, 150, 24, 255));
                       });

        if (dialoguesController->hasPlayerWithRabbitLastDialogueFinished())
        {
            storyGameState->gameFinishedSuccessfully();
        }
    }

    if (playerDead and deadTimer->getElapsedSeconds() > deadTime)
    {
        storyGameState->gameOver();
    }

    if (playerTalkedToDruid and dialoguesController->hasPlayerWithDruidFirstDialogueFinished())
    {
        mainCharacters.npc->getComponent<components::core::DialogueTextComponent>()->setText(
            "Press E to talk");
        mainCharacters.npc->getComponent<components::core::LimitedSpaceActionComponent>()->setAction(
            [this]() { druidSecondAction(); });
        playerTalkedToDruid = false;
    }

    dialoguesController->update(input);
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

void Level1Controller::firstCampfireAction()
{
    sleepTimer->restart();
    playerSleeping = true;
    mainCharacters.player->getComponent<components::core::AnimationComponent>()->setAnimation(
        animations::AnimationType::Sleep);
    mainCharacters.player->getComponent<components::core::MovementComponent>()->lock();
}

void Level1Controller::lastCampfireAction()
{
    sleepTimer->restart();
    playerSleeping = true;
    playerSleepingNextToLastCampfire = true;
    mainCharacters.player->getComponent<components::core::AnimationComponent>()->setAnimation(
        animations::AnimationType::Sleep);
    mainCharacters.player->getComponent<components::core::MovementComponent>()->lock();
}

void Level1Controller::druidFirstAction()
{
    dialoguesController->startPlayerWithDruidFirstDialogue();
    playerTalkedToDruid = true;
}

void Level1Controller::druidSecondAction()
{
    dialoguesController->startPlayerWithDruidSecondDialogue();
}

void Level1Controller::deadPlayerAction()
{
    deadTimer->restart();
    playerDead = true;
    mainCharacters.player->getComponent<components::core::AnimationComponent>()->forceAnimation(
        animations::AnimationType::Sleep);
    mainCharacters.player->getComponent<components::core::MovementComponent>()->lock();
}

}