#include "ControlsState.h"

#include "ControlsStateUIConfigBuilder.h"
#include "ui/DefaultUIManager.h"

namespace game
{

ControlsState::ControlsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit,
                             std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      inputStatus{nullptr},
      uiManager{std::move(uiManagerInit)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(ControlsStateUIConfigBuilder::createControlsUIConfig(this));
}

ControlsState::~ControlsState()
{
    inputManager->removeObserver(this);
}

NextState ControlsState::update(const utils::DeltaTime& deltaTime)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime);
    return NextState::Same;
}

void ControlsState::lateUpdate(const utils::DeltaTime& deltaTime) {}

void ControlsState::render()
{
    rendererPool->renderAll();
}

std::string ControlsState::getName() const
{
    return "Controls state";
}

void ControlsState::activate()
{
    active = true;
    uiManager->activate();
}

void ControlsState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void ControlsState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

}