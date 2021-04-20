#include "StateFactory.h"

#include <utility>

#include "ChooseMapState.h"
#include "ControlsState.h"
#include "EditorMenuState.h"
#include "EditorState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SaveMapState.h"
#include "SettingsState.h"
#include "exceptions/StateTypeNotFound.h"

namespace game
{
StateFactory::StateFactory(std::shared_ptr<window::Window> windowInit,
                           std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           TileMap& tileMapInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
      fileAccess{std::move(fileAccessInit)},
      states{statesInit},
      tileMap{tileMapInit}
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
                                             tileMap);
    }
    case StateType::Game:
    {
        return std::make_unique<GameState>(window, rendererPool, fileAccess, states,
                                           std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::Menu:
    {
        return std::make_unique<MenuState>(window, rendererPool, fileAccess, states,
                                           std::make_unique<components::ui::DefaultUIManager>(rendererPool));
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
        return std::make_unique<SettingsState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::ChooseMap:
    {
        return std::make_unique<ChooseMapState>(
            window, rendererPool, fileAccess, states,
            std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    default:
    {
        throw exceptions::StateTypeNotFound{"no information how to create: " + toString(stateType)};
    }
    }
}

}