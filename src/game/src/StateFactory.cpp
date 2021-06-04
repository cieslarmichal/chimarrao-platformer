#include "StateFactory.h"

#include <utility>

#include "ChooseMapState.h"
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

namespace
{
const auto menuButtonColor = graphics::Color(251, 190, 102);
const auto menuButtonHoverColor = graphics::Color(205, 128, 66);
const auto settingsButtonColor = menuButtonColor;
const auto settingsButtonHoverColor = menuButtonHoverColor;
}

StateFactory::StateFactory(std::shared_ptr<window::Window> windowInit,
                           std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           std::shared_ptr<TileMap> tileMapInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
      fileAccess{std::move(fileAccessInit)},
      states{statesInit},
      tileMap{std::move(tileMapInit)},
      collisionSystemFactory{physics::CollisionSystemFactory::createCollisionSystemFactory()}
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
            tileMap);
    }
    case StateType::Menu:
    {
        auto uiManager = std::make_shared<components::ui::DefaultUIManager>(rendererPool);
        auto buttonsNavigator = std::make_unique<PaginatedButtonsNavigator>(
            uiManager, MenuStateUIConfigBuilder::getButtonNames(), MenuStateUIConfigBuilder::getIconNames(),
            menuButtonColor, menuButtonHoverColor, utils::TimerFactory::createTimer());
        return std::make_unique<MenuState>(window, rendererPool, fileAccess, states, uiManager,
                                           std::move(buttonsNavigator));
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
        auto buttonsNavigator = std::make_unique<GridButtonsNavigator>(
            uiManager, SettingsStateUIConfigBuilder::getGridButtonsInfo(),
            SettingsStateUIConfigBuilder::getIconNames(), settingsButtonColor, settingsButtonHoverColor,
            utils::TimerFactory::createTimer());
        return std::make_unique<SettingsState>(window, rendererPool, fileAccess, states, uiManager,
                                               std::move(buttonsNavigator));
    }
    case StateType::ChooseMap:
    {
        return std::make_unique<ChooseMapState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool),
            std::make_unique<FileSystemMapsReader>(fileAccess), tileMap);
    }
    default:
    {
        throw exceptions::StateTypeNotFound{"no information how to create: " + toString(stateType)};
    }
    }
}

}