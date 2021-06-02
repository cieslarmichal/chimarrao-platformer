#include "PaginatedButtonsNavigator.h"

namespace game
{

PaginatedButtonsNavigator::PaginatedButtonsNavigator(components::ui::UIManager& uiManager,
                                                     const std::vector<std::string>& buttonNames,
                                                     const std::vector<std::string>& iconNames,
                                                     graphics::Color buttonsDefaultColor,
                                                     graphics::Color buttonsHoverColor)
    : uiManager{uiManager},
      buttonNames{buttonNames},
      iconNames{iconNames},
      timeAfterButtonCanBeSwitched{0.1f},
      buttonsDefaultColor{buttonsDefaultColor},
      buttonsHoverColor{buttonsHoverColor}
{
    uiManager.setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::update(const utils::DeltaTime&, const input::Input& input)
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
        uiManager.invokeClickAction(buttonNames.at(currentItemIndex), input::InputKey::MouseLeft);
    }
}

void PaginatedButtonsNavigator::activate()
{
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::setFocusOnItem(unsigned int itemIndex)
{
    if (itemIndex < 0 or itemIndex >= buttonNames.size())
    {
        return;
    }

    unselectAllButtons();
    changeSelectedButton(itemIndex);
}

void PaginatedButtonsNavigator::loseFocus()
{
    unselectAllButtons();
    hideIcons();
}

void PaginatedButtonsNavigator::changeSelectedButtonUp()
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

    uiManager.setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::changeSelectedButtonDown()
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
    uiManager.setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::changeSelectedButton(unsigned int buttonIndex)
{
    currentItemIndex = buttonIndex;
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::unselectAllButtons()
{
    for (const auto& buttonName : buttonNames)
    {
        uiManager.setColor(buttonName, buttonsDefaultColor);
    }
}

void PaginatedButtonsNavigator::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    uiManager.activateComponent(iconNames[iconIndex]);
}

void PaginatedButtonsNavigator::hideIcons()
{
    for (auto& iconName : iconNames)
    {
        uiManager.deactivateComponent(iconName);
    }
}

}