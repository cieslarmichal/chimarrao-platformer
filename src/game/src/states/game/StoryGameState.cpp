#include "StoryGameState.h"

#include <utility>

#include "DefaultComponentOwnersManager.h"
#include "DefaultUIManager.h"
#include "GameStateUIConfigBuilder.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "Level1Controller.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"

namespace game
{

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto soundtrackPath = projectPath + "resources/game_music_loop.wav";
}

StoryGameState::StoryGameState(const std::shared_ptr<window::Window>& windowInit,
                               const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                               std::shared_ptr<components::ui::UIManager> uiManagerInit,
                               std::shared_ptr<TileMap> tileMapInit,
                               const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
                               std::shared_ptr<audio::MusicManager> musicManagerInit,
                               std::unique_ptr<physics::PhysicsFactory> physicsFactoryInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      tileMap{std::move(tileMapInit)},
      sharedContext{sharedContextInit},
      musicManager{std::move(musicManagerInit)},
      physicsFactory{std::move(physicsFactoryInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig());

    auto ownersManager = std::make_shared<components::core::DefaultComponentOwnersManager>(
        physicsFactory->createCollisionSystem());
    auto rayCast = physicsFactory->createRayCast();
    auto quadTree = physicsFactory->getQuadTree();
    auto characterFactory =
        std::make_shared<CharacterFactory>(sharedContext, tileMap, rayCast, quadTree, ownersManager);
    auto obstacleFactory = std::make_shared<ObstacleFactory>(sharedContext);
    auto itemFactory = std::make_shared<ItemFactory>(sharedContext);

    auto level1Controller =
        std::make_unique<Level1Controller>(tileMap, ownersManager, characterFactory, obstacleFactory,
                                           itemFactory, sharedContext, fileAccess, uiManager, this);
    const auto player = level1Controller->getCharacters().player;

    levelControllers.push(std::move(level1Controller));

    hud = std::make_unique<HeadsUpDisplay>(player, sharedContext,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig(),
                                           utils::TimerFactory::createTimer());

    timer = utils::TimerFactory::createTimer();

    musicId = musicManager->acquire(soundtrackPath);
    musicManager->play(musicId);
    musicManager->setVolume(musicId, 3);
}

NextState StoryGameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        levelControllers.front()->update(deltaTime, input);
        uiManager->update(deltaTime, input);
        hud->update(deltaTime, input);
    }

    return NextState::Same;
}

void StoryGameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void StoryGameState::render()
{
    rendererPool->renderAll();
}

StateType StoryGameState::getType() const
{
    return StateType::StoryGame;
}

void StoryGameState::activate()
{
    active = true;
    paused = false;
    timer->restart();
    levelControllers.front()->activate();
    uiManager->activate();
    musicManager->play(musicId);
}

void StoryGameState::deactivate()
{
    active = false;
    timer->restart();
    levelControllers.front()->deactivate();
    uiManager->deactivate();
    musicManager->stop(musicId);
}

void StoryGameState::pause()
{
    paused = true;
    levelControllers.front()->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::Pause);
}

void StoryGameState::gameOver()
{
    paused = true;
    levelControllers.front()->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::GameOver);
}

void StoryGameState::gameFinishedSuccessfully()
{
    paused = true;
    levelControllers.front()->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::GameVictory);
}

}
