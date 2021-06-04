#include "PaginatedButtonsNavigator.h"

namespace game
{

PaginatedButtonsNavigator::PaginatedButtonsNavigator(std::shared_ptr<components::ui::UIManager> uiManager,
                                                     const std::vector<std::string>& buttonNames,
                                                     const std::vector<std::string>& iconNames,
                                                     graphics::Color buttonsDefaultColor,
                                                     graphics::Color buttonsHoverColor,
                                                     std::unique_ptr<utils::Timer> moveTimer,
                                                     std::unique_ptr<utils::Timer> actionTimer)
    : uiManager{std::move(uiManager)},
      buttonNames{buttonNames},
      buttonNamesWithIndices{getButtonNamesWithIndices()},
      iconNames{iconNames},
      buttonsDefaultColor{buttonsDefaultColor},
      buttonsHoverColor{buttonsHoverColor},
      timeAfterButtonCanBeSwitched{0.1f},
      timeAfterActionCanBeExecuted{0.1f},
      switchButtonTimer{std::move(moveTimer)},
      actionTimer{std::move(actionTimer)}
{
    if (buttonNames.size() != iconNames.size())
    {
        throw std::runtime_error{"Icons size is not equal buttons size"};
    }
}

void PaginatedButtonsNavigator::initialize()
{
    uiManager->setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

NextState PaginatedButtonsNavigator::update(const utils::DeltaTime&, const input::Input& input)
{
    if (switchButtonTimer->getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        if (input.isKeyPressed(input::InputKey::Up))
        {
            changeSelectedButtonUp();
        }
        else if (input.isKeyPressed(input::InputKey::Down))
        {
            changeSelectedButtonDown();
        }
        else if (input.isKeyPressed(input::InputKey::Left))
        {
        }
        else if (input.isKeyPressed(input::InputKey::Right))
        {
        }

        switchButtonTimer->restart();
    }

    if (actionTimer->getElapsedSeconds() > timeAfterActionCanBeExecuted)
    {
        if (input.isKeyPressed(input::InputKey::Enter))
        {
            uiManager->invokeClickAction(buttonNames.at(currentItemIndex), input::InputKey::MouseLeft);
        }
        else if (input.isKeyPressed(input::InputKey::Escape))
        {
            return NextState::Previous;
        }

        actionTimer->restart();
    }

    return NextState::Same;
}

void PaginatedButtonsNavigator::activate()
{
    setIconVisible(currentItemIndex);
    switchButtonTimer->restart();
    actionTimer->restart();
}

void PaginatedButtonsNavigator::setFocusOnButton(const std::string& buttonName)
{
    if (buttonNamesWithIndices.contains(buttonName))
    {
        unselectAllButtons();
        changeSelectedButton(buttonNamesWithIndices.at(buttonName));
    }
}

void PaginatedButtonsNavigator::loseFocus()
{
    unselectAllButtons();
    hideIcons();
}

std::unordered_map<std::string, unsigned> PaginatedButtonsNavigator::getButtonNamesWithIndices()
{
    std::unordered_map<std::string, unsigned> buttonNamesWithIndicesInit;

    for (std::size_t index = 0; index < buttonNames.size(); ++index)
    {
        buttonNamesWithIndicesInit.insert({buttonNames[index], index});
    }
    return buttonNamesWithIndicesInit;
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

    uiManager->setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
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
    uiManager->setColor(buttonNames.at(currentItemIndex), buttonsHoverColor);
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
        uiManager->setColor(buttonName, buttonsDefaultColor);
    }
}

void PaginatedButtonsNavigator::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    uiManager->activateComponent(iconNames[iconIndex]);
}

void PaginatedButtonsNavigator::hideIcons()
{
    for (auto& iconName : iconNames)
    {
        uiManager->deactivateComponent(iconName);
    }
}

}