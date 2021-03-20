#include "ControlsState.h"

#include "ui/DefaultUIManager.h"
#include "ControlsStateUIConfigBuilder.h"

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

void ControlsState::update(const utils::DeltaTime& deltaTime)
{
    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    uiManager->update(deltaTime);
}

void ControlsState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

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

void ControlsState::backToMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

}