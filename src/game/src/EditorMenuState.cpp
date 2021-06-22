#include "EditorMenuState.h"

#include <utility>

#include "EditorMenuStateUIConfigBuilder.h"
#include "TimerFactory.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                                 std::shared_ptr<components::ui::UIManager> uiManagerInit,
                                 std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{std::move(tileMapInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, EditorMenuStateUIConfigBuilder::getGridButtonsInfo(),
          EditorMenuStateUIConfigBuilder::getIconNames(), buttonColor, buttonHoverColor,
          utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer())}
{
    uiManager->createUI(EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(this));
    buttonsNavigator->initialize();
    possibleLeaveFromStateTimer = utils::TimerFactory::createTimer();
}

NextState EditorMenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Previous;
    }

    if (shouldBackToEditor)
    {
        return NextState::Previous;
    }

    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void EditorMenuState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

StateType EditorMenuState::getType() const
{
    return StateType::EditorMenu;
}

void EditorMenuState::activate()
{
    active = true;
    uiManager->activate();
    buttonsNavigator->activate();
    possibleLeaveFromStateTimer->restart();
}

void EditorMenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

}