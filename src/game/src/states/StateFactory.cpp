#include "StateFactory.h"

#include <utility>

#include "ChooseMapState.h"
#include "ChooseMapStateUIConfigBuilder.h"
#include "ControlsState.h"
#include "CustomGameState.h"
#include "DefaultComponentOwnersManager.h"
#include "EditorMenuState.h"
#include "EditorState.h"
#include "FileSystemMapsReader.h"
#include "GameOverState.h"
#include "GameVictoryState.h"
#include "PhysicsFactory.h"
#include "SaveMapState.h"
#include "SettingsState.h"
#include "SettingsStateUIConfigBuilder.h"
#include "StoryGameState.h"
#include "TimerFactory.h"
#include "menu/MenuState.h"
#include "menu/MenuStateUIConfigBuilder.h"
#include "pause/PauseState.h"

namespace game
{

StateFactory::StateFactory(std::shared_ptr<window::Window> windowInit,
                           std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           std::shared_ptr<TileMap> tileMapInit,
                           std::shared_ptr<audio::MusicManager> musicManagerInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
      fileAccess{std::move(fileAccessInit)},
      states{statesInit},
      tileMap{std::move(tileMapInit)},
      sharedContext{std::make_shared<components::core::SharedContext>(rendererPool)},
      musicManager{std::move(musicManagerInit)}
{
}

std::unique_ptr<State> StateFactory::createState(StateType stateType)
{
    switch (stateType)
    {
    case StateType::Controls:
    {
        return std::make_unique<ControlsState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext));
    }
    case StateType::EditorMenu:
    {
        return std::make_unique<EditorMenuState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext), tileMap);
    }
    case StateType::Editor:
    {
        const auto mapBoundaries = utils::FloatRect(0, 0, 80, 60);
        auto collisionSystemFactory = physics::PhysicsFactory::createPhysicsFactory(mapBoundaries);

        return std::make_unique<EditorState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext), tileMap, sharedContext,
            std::make_unique<components::core::DefaultComponentOwnersManager>(
                collisionSystemFactory->createCollisionSystem()));
    }
    case StateType::CustomGame:
    {
        const auto tileMapSize = tileMap->getSize();
        const auto mapBoundaries =
            utils::FloatRect(0, 0, static_cast<float>(tileMapSize.x), static_cast<float>(tileMapSize.y));
        auto physicsFactory = physics::PhysicsFactory::createPhysicsFactory(mapBoundaries);

        return std::make_unique<CustomGameState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext), tileMap, sharedContext,
            musicManager, std::move(physicsFactory));
    }
    case StateType::StoryGame:
    {
        const auto tileMapSize = tileMap->getSize();
        const auto mapBoundaries =
            utils::FloatRect(0, 0, static_cast<float>(tileMapSize.x), static_cast<float>(tileMapSize.y));
        auto physicsFactory = physics::PhysicsFactory::createPhysicsFactory(mapBoundaries);

        return std::make_unique<StoryGameState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext), tileMap, sharedContext,
            musicManager, std::move(physicsFactory));
    }
    case StateType::Menu:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(sharedContext);
        return std::make_unique<MenuState>(window, rendererPool, fileAccess, states, uiManager);
    }
    case StateType::Pause:
    {
        return std::make_unique<PauseState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext));
    }
    case StateType::GameOver:
    {
        return std::make_unique<GameOverState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext));
    }
    case StateType::GameVictory:
    {
        return std::make_unique<GameVictoryState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext));
    }
    case StateType::SaveMap:
    {
        return std::make_unique<SaveMapState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(sharedContext), tileMap);
    }
    case StateType::Settings:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(sharedContext);
        return std::make_unique<SettingsState>(window, rendererPool, fileAccess, states, uiManager);
    }
    case StateType::ChooseMap:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(sharedContext);
        return std::make_unique<ChooseMapState>(window, rendererPool, fileAccess, states, uiManager, tileMap,
                                                std::make_unique<FileSystemMapsReader>(fileAccess));
    }
    default:
    {
        throw exceptions::StateTypeNotFound{"no information how to create: " + toString(stateType)};
    }
    }
}

}