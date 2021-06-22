#include "GridButtonsNavigator.h"

namespace game
{

GridButtonsNavigator::GridButtonsNavigator(std::shared_ptr<components::ui::UIManager> uiManager,
                                           const std::vector<std::vector<GridButtonInfo>>& gridButtonsInfo,
                                           const std::vector<std::string>& iconNames,
                                           graphics::Color buttonsDefaultColor,
                                           graphics::Color buttonsHoverColor,
                                           std::unique_ptr<utils::Timer> switchButtonTimer,
                                           std::unique_ptr<utils::Timer> actionTimer)
    : uiManager{std::move(uiManager)},
      gridButtonsInfo{gridButtonsInfo},
      buttonNamesWithIndices{getButtonNamesWithIndices()},
      iconNames{iconNames},
      buttonsDefaultColor{buttonsDefaultColor},
      buttonsHoverColor{buttonsHoverColor},
      timeAfterButtonCanBeSwitched{0.1f},
      timeAfterActionCanBeExecuted{0.1f},
      switchButtonTimer{std::move(switchButtonTimer)},
      actionTimer{std::move(actionTimer)}
{
    for (const auto& rowOfGridButtonInfo : gridButtonsInfo)
    {
        const auto rowValid = std::all_of(rowOfGridButtonInfo.begin(), rowOfGridButtonInfo.end(),
                                          [&](const GridButtonInfo& gridButtonInfo)
                                          { return gridButtonInfo.iconIndex < iconNames.size(); });
        if (not rowValid)
        {
            throw std::runtime_error{"Incorrect grid buttons info: icon index not within icon names indices"};
        }
    }
}

void GridButtonsNavigator::initialize()
{
    if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].highlightsWhenSelected)
    {
        uiManager->setColor(gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                            buttonsHoverColor);
    }

    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

NextState GridButtonsNavigator::update(const utils::DeltaTime&, const input::Input& input)
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
            changeSelectedButtonLeft();
        }
        else if (input.isKeyPressed(input::InputKey::Right))
        {
            changeSelectedButtonRight();
        }

        switchButtonTimer->restart();
    }

    if (actionTimer->getElapsedSeconds() > timeAfterActionCanBeExecuted)
    {
        if (input.isKeyPressed(input::InputKey::Enter))
        {
            uiManager->invokeClickAction(
                gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                input::InputKey::MouseLeft);
        }
        else if (input.isKeyPressed(input::InputKey::Escape))
        {
            return NextState::Previous;
        }

        actionTimer->restart();
    }

    return NextState::Same;
}

void GridButtonsNavigator::activate()
{
    setIconAssociatedWithButtonVisible(currentButtonIndex);
    switchButtonTimer->restart();
    actionTimer->restart();
}

void GridButtonsNavigator::setFocusOnButton(const std::string& buttonName)
{
    if (buttonNamesWithIndices.contains(buttonName))
    {
        unselectAllButtons();
        changeSelectedButton(buttonNamesWithIndices.at(buttonName));
    }
}

void GridButtonsNavigator::loseFocus()
{
    unselectAllButtons();
    hideIcons();
}

std::unordered_map<std::string, utils::Vector2u> GridButtonsNavigator::getButtonNamesWithIndices()
{
    std::unordered_map<std::string, utils::Vector2u> buttonNamesWithIndicesInit;

    for (std::size_t y = 0; y < gridButtonsInfo.size(); ++y)
    {
        for (std::size_t x = 0; x < gridButtonsInfo[y].size(); ++x)
        {
            buttonNamesWithIndicesInit.insert(
                {gridButtonsInfo[y][x].buttonName,
                 utils::Vector2u{static_cast<unsigned>(x), static_cast<unsigned>(y)}});
        }
    }

    return buttonNamesWithIndicesInit;
}

void GridButtonsNavigator::changeSelectedButtonUp()
{
    const auto previousButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    unselectAllButtons();

    if (currentButtonIndex.y == 0)
    {
        currentButtonIndex.y = static_cast<unsigned int>(gridButtonsInfo.size() - 1);
    }
    else
    {
        --currentButtonIndex.y;
    }

    currentButtonIndex.x = 0;

    bool selectedButtonChanged = false;
    for (const auto& horizontalButton : gridButtonsInfo[currentButtonIndex.y])
    {
        if (horizontalButton.verticalMoveKeepButtonSelected)
        {
            selectedButtonChanged = true;
            break;
        }
    }

    if (not selectedButtonChanged &&
        gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].highlightsWhenSelected)
    {
        uiManager->setColor(gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                            buttonsHoverColor);
    }

    const auto currentButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (previousButtonName != currentButtonName)
    {
        uiManager->invokeMouseOutAction(previousButtonName);
    }

    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

void GridButtonsNavigator::changeSelectedButtonDown()
{
    const auto previousButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    unselectAllButtons();

    if (currentButtonIndex.y == gridButtonsInfo.size() - 1)
    {
        currentButtonIndex.y = 0;
    }
    else
    {
        ++currentButtonIndex.y;
    }
    currentButtonIndex.x = 0;

    bool selectedButtonChanged = false;
    for (const auto& horizontalButton : gridButtonsInfo[currentButtonIndex.y])
    {
        if (horizontalButton.verticalMoveKeepButtonSelected)
        {
            selectedButtonChanged = true;
            break;
        }
    }

    if (not selectedButtonChanged &&
        gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].highlightsWhenSelected)
    {
        uiManager->setColor(gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                            buttonsHoverColor);
    }

    const auto currentButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (previousButtonName != currentButtonName)
    {
        uiManager->invokeMouseOutAction(previousButtonName);
    }

    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

void GridButtonsNavigator::changeSelectedButtonLeft()
{
    const auto previousButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (currentButtonIndex.x == 0)
    {
        if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].horizontalMoveCauseAction)
        {
            uiManager->invokeClickAction(
                gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                input::InputKey::MouseLeft);
        }
        else
        {
            currentButtonIndex.x =
                static_cast<unsigned int>(gridButtonsInfo[currentButtonIndex.y].size() - 1);
        }
    }
    else
    {
        --currentButtonIndex.x;
        if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].horizontalMoveCauseAction)
        {
            uiManager->invokeClickAction(
                gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                input::InputKey::MouseLeft);
        }
    }

    unselectAllButtons();

    if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].highlightsWhenSelected)
    {
        uiManager->setColor(gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                            buttonsHoverColor);
    }

    const auto currentButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (previousButtonName != currentButtonName)
    {
        uiManager->invokeMouseOutAction(previousButtonName);
    }

    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

void GridButtonsNavigator::changeSelectedButtonRight()
{
    const auto previousButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (currentButtonIndex.x == gridButtonsInfo[currentButtonIndex.y].size() - 1)
    {
        if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].horizontalMoveCauseAction)
        {
            uiManager->invokeClickAction(
                gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                input::InputKey::MouseLeft);
        }
        else
        {
            currentButtonIndex.x = 0;
        }
    }
    else
    {
        ++currentButtonIndex.x;
        if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].horizontalMoveCauseAction)
        {
            uiManager->invokeClickAction(
                gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                input::InputKey::MouseLeft);
        }
    }

    unselectAllButtons();

    if (gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].highlightsWhenSelected)
    {
        uiManager->setColor(gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName,
                            buttonsHoverColor);
    }

    const auto currentButtonName = gridButtonsInfo[currentButtonIndex.y][currentButtonIndex.x].buttonName;

    if (previousButtonName != currentButtonName)
    {
        uiManager->invokeMouseOutAction(previousButtonName);
    }

    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

void GridButtonsNavigator::changeSelectedButton(const utils::Vector2u& buttonIndex)
{
    currentButtonIndex = buttonIndex;
    setIconAssociatedWithButtonVisible(currentButtonIndex);
}

void GridButtonsNavigator::unselectAllButtons()
{
    for (const auto& rowOfGridButtonsInfo : gridButtonsInfo)
    {
        for (const auto& gridButtonInfo : rowOfGridButtonsInfo)
        {
            if (not gridButtonInfo.verticalMoveKeepButtonSelected)
            {
                uiManager->setColor(gridButtonInfo.buttonName, buttonsDefaultColor);
            }
        }
    }
}

void GridButtonsNavigator::setIconAssociatedWithButtonVisible(const utils::Vector2u& buttonIndex)
{
    hideIcons();
    uiManager->activateComponent(iconNames[gridButtonsInfo[buttonIndex.y][buttonIndex.x].iconIndex]);
}

void GridButtonsNavigator::hideIcons()
{
    for (auto& iconName : iconNames)
    {
        uiManager->deactivateComponent(iconName);
    }
}
}