#include "ControlsState.h"

#include <utility>

#include "ControlsStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

ControlsState::ControlsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                             std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, ControlsStateUIConfigBuilder::getGridButtonsInfo(),
          ControlsStateUIConfigBuilder::getIconNames(), buttonColor, buttonHoverColor,
          utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer())}
{
    uiManager->createUI(ControlsStateUIConfigBuilder::createControlsUIConfig(this));
    buttonsNavigator->initialize();
}

NextState ControlsState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Exit;
    }

    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void ControlsState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void ControlsState::render()
{
    rendererPool->renderAll();
}

StateType ControlsState::getType() const
{
    return StateType::Controls;
}

void ControlsState::activate()
{
    active = true;
    uiManager->activate();
    buttonsNavigator->activate();
}

void ControlsState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

}