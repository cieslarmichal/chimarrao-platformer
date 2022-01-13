#include "CustomGameState.h"

#include <utility>

#include "CustomWorldBuilder.h"
#include "DefaultComponentOwnersManager.h"
#include "DefaultUIManager.h"
#include "GameStateUIConfigBuilder.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"

namespace game
{

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto soundtrackPath = projectPath + "resources/game_music_loop.wav";
}

CustomGameState::CustomGameState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                                 std::shared_ptr<components::ui::UIManager> uiManagerInit,
                                 std::shared_ptr<TileMap> tileMapInit,
                                 const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
                                 std::shared_ptr<audio::MusicManager> musicManagerInit,
                                 std::unique_ptr<physics::PhysicsFactory> physicsFactory)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      tileMap{std::move(tileMapInit)},
      sharedContext{sharedContextInit},
      musicManager{std::move(musicManagerInit)},
      ownersManager{std::make_shared<components::core::DefaultComponentOwnersManager>(
          physicsFactory->createCollisionSystem())}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig());

    auto rayCast = physicsFactory->createRayCast();
    auto quadTree = physicsFactory->getQuadTree();
    auto characterFactory = std::make_shared<CharacterFactory>(sharedContext, tileMap, rayCast, quadTree);
    auto obstacleFactory = std::make_shared<ObstacleFactory>(sharedContext);
    auto worldBuilder =
        std::make_unique<CustomWorldBuilder>(characterFactory, obstacleFactory, sharedContext);

    const auto worldObjects = worldBuilder->buildWorldObjects(tileMap);
    const auto player = worldBuilder->getPlayer();

    hud = std::make_unique<HeadsUpDisplay>(player, sharedContext,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig(),
                                           utils::TimerFactory::createTimer());

    for (const auto& worldObject : worldObjects)
    {
        ownersManager->add(worldObject);
    }

    timer = utils::TimerFactory::createTimer();

    musicId = musicManager->acquire(soundtrackPath);
    musicManager->play(musicId);
    musicManager->setVolume(musicId, 15);
}

NextState CustomGameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        ownersManager->update(deltaTime, input);
        uiManager->update(deltaTime, input);
        hud->update(deltaTime, input);
    }

    return NextState::Same;
}

void CustomGameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void CustomGameState::render()
{
    rendererPool->renderAll();
}

StateType CustomGameState::getType() const
{
    return StateType::CustomGame;
}

void CustomGameState::activate()
{
    active = true;
    paused = false;
    timer->restart();
    ownersManager->activate();
    uiManager->activate();
    musicManager->play(musicId);
}

void CustomGameState::deactivate()
{
    active = false;
    timer->restart();
    ownersManager->deactivate();
    uiManager->deactivate();
    musicManager->stop(musicId);
}

void CustomGameState::pause()
{
    paused = true;
    ownersManager->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::Pause);
}

}
