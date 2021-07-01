#include "PauseState.h"

#include <utility>

#include "PauseStateUIConfigBuilder.h"
#include "TimerFactory.h"
#include "ui/DefaultUIManager.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

PauseState::PauseState(const std::shared_ptr<window::Window>& windowInit,
                       const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                       std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                       std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToGame{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, PauseStateUIConfigBuilder::getGridButtonsInfo(),
          PauseStateUIConfigBuilder::getIconNames(), buttonColor, buttonHoverColor,
          utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer())}
{
    uiManager->createUI(PauseStateUIConfigBuilder::createPauseUIConfig(this));
    buttonsNavigator->initialize();
}

NextState PauseState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Previous;
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
    buttonsNavigator->activate();
}

void PauseState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void PauseState::backToGame()
{
    shouldBackToGame = true;
}

void PauseState::backToMenu()
{
    shouldBackToMenu = true;
}

}