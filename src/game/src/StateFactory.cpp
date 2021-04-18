#include "StateFactory.h"

#include <utility>

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
                           std::shared_ptr<graphics::RendererPool> rendererPoolInit, States& statesInit,
                           TileMap& tileMapInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
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
            window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::EditorMenu:
    {
        return std::make_unique<EditorMenuState>(
            window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool),
            tileMap);
    }
    case StateType::Editor:
    {
        return std::make_unique<EditorState>(window, rendererPool, states,
                                             std::make_unique<components::ui::DefaultUIManager>(rendererPool),
                                             tileMap);
    }
    case StateType::Game:
    {
        return std::make_unique<GameState>(window, rendererPool, states,
                                           std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::Menu:
    {
        return std::make_unique<MenuState>(window, rendererPool, states,
                                           std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::Pause:
    {
        return std::make_unique<PauseState>(window, rendererPool, states,
                                            std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    case StateType::SaveMap:
    {
        return std::make_unique<SaveMapState>(
            window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool),
            tileMap);
    }
    case StateType::Settings:
    {
        return std::make_unique<SettingsState>(
            window, rendererPool, states, std::make_unique<components::ui::DefaultUIManager>(rendererPool));
    }
    default:
    {
        throw exceptions::StateTypeNotFound{"no information how to create: " + toString(stateType)};
    }
    }
}

}