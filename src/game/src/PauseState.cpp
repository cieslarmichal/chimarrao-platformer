#include "PauseState.h"

#include <utility>

#include "PauseStateUIConfigBuilder.h"
#include "TimerFactory.h"
#include "ui/DefaultUIManager.h"

namespace game
{

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                       std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    uiManager->createUI(PauseStateUIConfigBuilder::createPauseUIConfig(this));
    timer = utils::TimerFactory::createTimer();
}

NextState PauseState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
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

void PauseState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

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