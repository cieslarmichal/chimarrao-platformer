#include "MenuState.h"

#include <utility>

#include "CommonUIConfigElements.h"
#include "MenuStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "core/GraphicsComponent.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto iconSize = utils::Vector2f{4, 4};
}

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      currentButtonIndex{0},
      timeAfterButtonCanBeSwitched{0.1f},
      buttonNames{"menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton",
                  "menuExitButton"},
      uiManager{std::move(uiManagerInit)},
      shouldExit{false},
      iconUniqueNames{"menuIcon1Image", "menuIcon2Image", "menuIcon3Image", "menuIcon4Image",
                      "menuIcon5Image"}
{
    uiManager->createUI(MenuStateUIConfigBuilder::createMenuUIConfig(this));
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

NextState MenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (switchButtonTimer.getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        if (input.isKeyPressed(input::InputKey::Up))
        {
            changeSelectedButtonUp();
        }
        else if (input.isKeyPressed(input::InputKey::Down))
        {
            changeSelectedButtonDown();
        }
        switchButtonTimer.restart();
    }

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
}

void MenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void MenuState::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentButtonIndex == 0)
    {
        currentButtonIndex = static_cast<unsigned int>(buttonNames.size() - 1);
    }
    else
    {
        currentButtonIndex--;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentButtonIndex == buttonNames.size() - 1)
    {
        currentButtonIndex = 0;
    }
    else
    {
        currentButtonIndex++;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButton(unsigned int buttonIndex)
{
    currentButtonIndex = buttonIndex;
    setIconVisible(currentButtonIndex);
}

void MenuState::unselectAllButtons()
{
    uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonColor);
}

void MenuState::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    uiManager->activateComponent(components::ui::UIComponentType::Image, iconUniqueNames[iconIndex]);
}

void MenuState::hideIcons()
{
    for (auto& iconUniqueName : iconUniqueNames)
    {
        uiManager->deactivateComponent(components::ui::UIComponentType::Image, iconUniqueName);
    }
}

}