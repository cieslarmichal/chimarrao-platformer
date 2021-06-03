#include "MenuState.h"

#include <utility>

#include "MenuStateUIConfigBuilder.h"
#include "PaginatedButtonsNavigator.h"

namespace game
{

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ButtonsNavigator> buttonsNavigator)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      buttonsNavigator{std::move(buttonsNavigator)},
      shouldExit{false}
{
    uiManager->createUI(MenuStateUIConfigBuilder::createMenuUIConfig(this));
    this->buttonsNavigator->initialize();
}

NextState MenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    buttonsNavigator->update(deltaTime, input);
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
    buttonsNavigator->activate();
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