#include "EditorMenuState.h"

#include "EditorMenuStateUIConfigBuilder.h"
#include "GetProjectPath.h"
#include "ui/DefaultUIManager.h"

namespace game
{

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<input::InputManager>& inputManagerInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 const std::shared_ptr<utils::FileAccess>& fileAccessInit,
                                 std::stack<std::unique_ptr<State>>& statesInit,
                                 std::unique_ptr<components::ui::UIManager> uiManagerInit, TileMap& tileMap)
    : State{windowInit, inputManagerInit, rendererPoolInit, fileAccessInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMap}
{
    inputManager->registerObserver(this);
    uiManager->createUI(EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(this));
    possibleLeaveFromStateTimer.start();
}

EditorMenuState::~EditorMenuState()
{
    inputManager->removeObserver(this);
}

NextState EditorMenuState::update(const utils::DeltaTime& deltaTime)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
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

    uiManager->update(deltaTime);
    return NextState::Same;
}

void EditorMenuState::lateUpdate(const utils::DeltaTime& deltaTime) {}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

std::string EditorMenuState::getName() const
{
    return "Editor menu state";
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

void EditorMenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

}