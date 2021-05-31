#include "MenuState.h"

#include <utility>

#include "MenuStateUIConfigBuilder.h"
#include "StateUINavigator.h"

namespace game
{

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      buttonNames{"menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton",
                  "menuExitButton"},
      iconNames{"menuIcon1Image", "menuIcon2Image", "menuIcon3Image", "menuIcon4Image", "menuIcon5Image"},
      shouldExit{false}
{
    uiManager->createUI(MenuStateUIConfigBuilder::createMenuUIConfig(this));
    uiNavigator = std::make_unique<StateUINavigator>(*uiManager, buttonNames, iconNames);
}

NextState MenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    uiNavigator->update(deltaTime, input);
    uiManager->update(deltaTime, input);

    if (shouldExit)
    {
        return NextState::Exit;
    }

    return NextState::Same;
}

void MenuState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void MenuState::render()
{
    rendererPool->renderAll();
}

StateType MenuState::getType() const
{
    return StateType::Menu;
}

void MenuState::activate()
{
    active = true;
    uiManager->activate();
    uiNavigator->activate();
}

void MenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void MenuState::handleWindowSizeChange(const utils::Vector2u& windowSize)
{
    State::handleWindowSizeChange(windowSize);
    uiManager->freezeAllButtons();
}

}