#include "PauseState.h"

#include "GetProjectPath.h"
#include "PauseStateUIConfigBuilder.h"
#include "ui/DefaultUIManager.h"

namespace game
{

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<input::InputManager>& inputManagerInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       const std::shared_ptr<utils::FileAccess>& fileAccessInit,
                       std::stack<std::unique_ptr<State>>& statesInit,
                       std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, fileAccessInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(PauseStateUIConfigBuilder::createPauseUIConfig(this));
    timer.start();
}

PauseState::~PauseState()
{
    inputManager->removeObserver(this);
}

NextState PauseState::update(const utils::DeltaTime& deltaTime)
{
    if (timer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToGame = true;
    }

    if (shouldBackToGame)
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

void PauseState::lateUpdate(const utils::DeltaTime& deltaTime) {}

void PauseState::render()
{
    rendererPool->renderAll();
}

std::string PauseState::getName() const
{
    return "Pause state";
}

void PauseState::activate()
{
    active = true;
    uiManager->activate();
}

void PauseState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void PauseState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

}