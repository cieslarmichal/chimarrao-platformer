#include "PauseState.h"

#include "GetProjectPath.h"
#include "PauseStateUIConfigBuilder.h"
#include "ui/DefaultUIManager.h"

namespace game
{

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit, States& statesInit,
                       std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    uiManager->createUI(PauseStateUIConfigBuilder::createPauseUIConfig(this));
    timer.start();
}

NextState PauseState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        input.isKeyPressed(input::InputKey::Escape))
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

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void PauseState::lateUpdate(const utils::DeltaTime&) {}

void PauseState::render()
{
    rendererPool->renderAll();
}

StateType PauseState::getType() const
{
    return StateType::Pause;
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

}