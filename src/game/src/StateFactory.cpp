#include "StateFactory.h"

#include <utility>

#include "ChooseMapState.h"
#include "ChooseMapStateUIConfigBuilder.h"
#include "ControlsState.h"
#include "DefaultComponentOwnersManager.h"
#include "EditorMenuState.h"
#include "EditorState.h"
#include "FileSystemMapsReader.h"
#include "GameState.h"
#include "GridButtonsNavigator.h"
#include "MenuState.h"
#include "MenuStateUIConfigBuilder.h"
#include "PaginatedButtonsNavigator.h"
#include "PauseState.h"
#include "SaveMapState.h"
#include "SettingsState.h"
#include "SettingsStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{

StateFactory::StateFactory(std::shared_ptr<window::Window> windowInit,
                           std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           std::shared_ptr<TileMap> tileMapInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
      fileAccess{std::move(fileAccessInit)},
      states{statesInit},
      tileMap{std::move(tileMapInit)},
      collisionSystemFactory{physics::PhysicsFactory::createCollisionSystemFactory()}
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
            std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::EditorMenu:
    {
        return std::make_unique<EditorMenuState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool), tileMap);
    }
    case StateType::Editor:
    {
        return std::make_unique<EditorState>(window, rendererPool, fileAccess, states,
                                             std::make_unique<components::ui::DefaultUIManager>(rendererPool),
                                             tileMap, utils::TimerFactory::createTimer());
    }
    case StateType::Game:
    {
        return std::make_unique<GameState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool),
            std::make_unique<DefaultComponentOwnersManager>(collisionSystemFactory->createCollisionSystem()),
            tileMap, collisionSystemFactory->createRayCast());
    }
    case StateType::Menu:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(rendererPool);
        return std::make_unique<MenuState>(window, rendererPool, fileAccess, states, uiManager);
    }
    case StateType::Pause:
    {
        return std::make_unique<PauseState>(window, rendererPool, fileAccess, states,
                                            std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::SaveMap:
    {
        return std::make_unique<SaveMapState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool), tileMap);
    }
    case StateType::Settings:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(rendererPool);
        return std::make_unique<SettingsState>(window, rendererPool, fileAccess, states, uiManager);
    }
    case StateType::ChooseMap:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(rendererPool);
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