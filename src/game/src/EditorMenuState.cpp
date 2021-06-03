#include "EditorMenuState.h"

#include <utility>

#include "EditorMenuStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "ui/DefaultUIManager.h"

namespace game
{

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
      tileMap{tileMapInit}
{
    uiManager->createUI(EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(this));
    possibleLeaveFromStateTimer.start();
}

NextState EditorMenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToEditor = true;
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
    possibleLeaveFromStateTimer.restart();
}

void EditorMenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

}