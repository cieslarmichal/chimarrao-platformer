#include "ControlsState.h"

#include "ControlsStateUIConfigBuilder.h"

namespace game
{

ControlsState::ControlsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             States& statesInit,
                             std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    uiManager->createUI(ControlsStateUIConfigBuilder::createControlsUIConfig(this));
}

NextState ControlsState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void ControlsState::lateUpdate(const utils::DeltaTime&) {}

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
}

void ControlsState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

}