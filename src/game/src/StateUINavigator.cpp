#include "StateUINavigator.h"

#include <utility>

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto iconSize = utils::Vector2f{4, 4};
}

StateUINavigator::StateUINavigator(components::ui::UIManager& uiManager,
                                   const std::vector<std::string>& buttonNames,
                                   const std::vector<std::string>& iconNames)
    : uiManager{uiManager}, buttonNames{buttonNames}, iconNames{iconNames}, timeAfterButtonCanBeSwitched{0.1f}
{
    uiManager.setColor(components::ui::UIComponentType::Button, buttonNames.at(currentItemIndex),
                       buttonHoverColor);
    setIconVisible(currentItemIndex);
}

void StateUINavigator::update(const utils::DeltaTime&, const input::Input& input)
{
    if (switchItemTimer.getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        if (input.isKeyPressed(input::InputKey::Up))
        {
            changeSelectedButtonUp();
        }
        else if (input.isKeyPressed(input::InputKey::Down))
        {
            changeSelectedButtonDown();
        }
        switchItemTimer.restart();
    }

    if (input.isKeyPressed(input::InputKey::Enter))
    {
        uiManager.invokeClickAction(components::ui::UIComponentType::Button, buttonNames.at(currentItemIndex),
                                    input::InputKey::MouseLeft);
    }
}

void StateUINavigator::activate()
{
    setIconVisible(currentItemIndex);
}

void StateUINavigator::setFocusOnItem(unsigned int itemIndex)
{
    if (itemIndex < 0 or itemIndex >= buttonNames.size())
    {
        return;
    }

    unselectAllButtons();
    changeSelectedButton(itemIndex);
}

void StateUINavigator::loseFocus()
{
    unselectAllButtons();
    hideIcons();
}

void StateUINavigator::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentItemIndex == 0)
    {
        currentItemIndex = static_cast<unsigned int>(buttonNames.size() - 1);
    }
    else
    {
        currentItemIndex--;
    }
    uiManager.setColor(components::ui::UIComponentType::Button, buttonNames.at(currentItemIndex),
                       buttonHoverColor);
    setIconVisible(currentItemIndex);
}

void StateUINavigator::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentItemIndex == buttonNames.size() - 1)
    {
        currentItemIndex = 0;
    }
    else
    {
        currentItemIndex++;
    }
    uiManager.setColor(components::ui::UIComponentType::Button, buttonNames.at(currentItemIndex),
                       buttonHoverColor);
    setIconVisible(currentItemIndex);
}

void StateUINavigator::changeSelectedButton(unsigned int buttonIndex)
{
    currentItemIndex = buttonIndex;
    setIconVisible(currentItemIndex);
}

void StateUINavigator::unselectAllButtons()
{
    uiManager.setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonColor);
    uiManager.setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonColor);
    uiManager.setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonColor);
    uiManager.setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonColor);
    uiManager.setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonColor);
}

void StateUINavigator::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    uiManager.activateComponent(components::ui::UIComponentType::Image, iconNames[iconIndex]);
}

void StateUINavigator::hideIcons()
{
    for (auto& iconName : iconNames)
    {
        uiManager.deactivateComponent(components::ui::UIComponentType::Image, iconName);
    }
}

}