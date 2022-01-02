#include "GameState.h"

#include <utility>

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

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ComponentOwnersManager> componentOwnersManagerInit,
                     std::shared_ptr<TileMap> tileMapInit,
                     const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
                     std::shared_ptr<audio::MusicManager> musicManagerInit,
                     std::unique_ptr<WorldBuilder> worldBuilderInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)},
      tileMap{std::move(tileMapInit)},
      sharedContext{sharedContextInit},
      musicManager{std::move(musicManagerInit)},
      worldBuilder{std::move(worldBuilderInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    const auto worldObjects = worldBuilder->buildWorldObjects(tileMap);
    const auto player = worldBuilder->getPlayer();

    hud = std::make_unique<HeadsUpDisplay>(player, sharedContext,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig(),
                                           utils::TimerFactory::createTimer());

    for (const auto& worldObject : worldObjects)
    {
        componentOwnersManager->add(worldObject);
    }

    componentOwnersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();

    musicId = musicManager->acquire(soundtrackPath);
    musicManager->play(musicId);
    musicManager->setVolume(musicId, 5);
}

NextState GameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        componentOwnersManager->update(deltaTime, input);
        uiManager->update(deltaTime, input);
        hud->update(deltaTime, input);
    }

    componentOwnersManager->processRemovals();

    return NextState::Same;
}

void GameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void GameState::render()
{
    rendererPool->renderAll();
}

StateType GameState::getType() const
{
    return StateType::Game;
}

void GameState::activate()
{
    active = true;
    paused = false;
    timer->restart();
    componentOwnersManager->activate();
    uiManager->activate();
    musicManager->play(musicId);
}

void GameState::deactivate()
{
    active = false;
    timer->restart();
    componentOwnersManager->deactivate();
    uiManager->deactivate();
    musicManager->stop(musicId);
}

void GameState::pause()
{
    paused = true;
    componentOwnersManager->deactivate();
    musicManager->pause(musicId);
    states.addNextState(StateType::Pause);
}

}
