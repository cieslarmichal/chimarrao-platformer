#include "GridButtonsNavigator.h"

namespace game
{

GridButtonsNavigator::GridButtonsNavigator(components::ui::UIManager& uiManager,
                                           const std::vector<std::vector<GridButtonInfo>>& gridButtonsInfo,
                                           const std::vector<std::string>& iconNames,
                                           graphics::Color buttonsDefaultColor,
                                           graphics::Color buttonsHoverColor)
    : uiManager{uiManager},
      gridButtonsInfo{gridButtonsInfo},
      availableGridButtonsIndices{getAvailableGridButtonsIndices()},
      iconNames{iconNames},
      timeAfterButtonCanBeSwitched{0.1f},
      buttonsDefaultColor{buttonsDefaultColor},
      buttonsHoverColor{buttonsHoverColor}
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

    uiManager.setColor(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName, buttonsHoverColor);
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::update(const utils::DeltaTime&, const input::Input& input)
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
        else if (input.isKeyPressed(input::InputKey::Left))
        {
            changeSelectedButtonLeft();
        }
        else if (input.isKeyPressed(input::InputKey::Right))
        {
            changeSelectedButtonRight();
        }
        switchItemTimer.restart();
    }

    if (input.isKeyPressed(input::InputKey::Enter))
    {
        uiManager.invokeClickAction(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName,
                                    input::InputKey::MouseLeft);
    }
}

void GridButtonsNavigator::activate()
{
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::setFocusOnItem(const utils::Vector2u& itemIndex)
{
    if (availableGridButtonsIndices.contains(itemIndex))
    {
        unselectAllButtons();
        changeSelectedButton(itemIndex);
    }
}

void GridButtonsNavigator::loseFocus()
{
    unselectAllButtons();
    hideIcons();
}

std::unordered_set<utils::Vector2u, ButtonIndexHash> GridButtonsNavigator::getAvailableGridButtonsIndices()
{
    std::unordered_set<utils::Vector2u, ButtonIndexHash> buttonsIndices;

    for (std::size_t y = 0; y < gridButtonsInfo.size(); ++y)
    {
        for (std::size_t x = 0; x < gridButtonsInfo[y].size(); ++x)
        {
            buttonsIndices.insert(utils::Vector2u{static_cast<unsigned>(x), static_cast<unsigned>(y)});
        }
    }
    return buttonsIndices;
}

void GridButtonsNavigator::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentItemIndex.y == 0)
    {
        currentItemIndex.y = static_cast<unsigned int>(gridButtonsInfo.size() - 1);
    }
    else
    {
        --currentItemIndex.y;
    }
    currentItemIndex.x = 0;
    uiManager.setColor(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName, buttonsHoverColor);
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentItemIndex.y == gridButtonsInfo.size() - 1)
    {
        currentItemIndex.y = 0;
    }
    else
    {
        ++currentItemIndex.y;
    }
    currentItemIndex.x = 0;

    uiManager.setColor(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName, buttonsHoverColor);
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::changeSelectedButtonLeft()
{
    unselectAllButtons();

    if (currentItemIndex.x == 0)
    {
        currentItemIndex.x = static_cast<unsigned int>(gridButtonsInfo[currentItemIndex.y].size() - 1);
    }
    else
    {
        --currentItemIndex.x;
    }

    uiManager.setColor(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName, buttonsHoverColor);
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::changeSelectedButtonRight()
{
    unselectAllButtons();

    if (currentItemIndex.x == gridButtonsInfo[currentItemIndex.y].size() - 1)
    {
        currentItemIndex.x = 0;
    }
    else
    {
        ++currentItemIndex.x;
    }

    uiManager.setColor(gridButtonsInfo[currentItemIndex.y][currentItemIndex.x].buttonName, buttonsHoverColor);
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::changeSelectedButton(const utils::Vector2u& buttonIndex)
{
    currentItemIndex = buttonIndex;
    setIconAssociatedWithButtonVisible(currentItemIndex);
}

void GridButtonsNavigator::unselectAllButtons()
{
    for (const auto& rowOfGridButtonsInfo : gridButtonsInfo)
    {
        for (const auto& gridButtonInfo : rowOfGridButtonsInfo)
        {
            uiManager.setColor(gridButtonInfo.buttonName, buttonsDefaultColor);
        }
    }
}

void GridButtonsNavigator::setIconAssociatedWithButtonVisible(const utils::Vector2u& buttonIndex)
{
    hideIcons();
    uiManager.activateComponent(iconNames[gridButtonsInfo[buttonIndex.y][buttonIndex.x].iconIndex]);
}

void GridButtonsNavigator::hideIcons()
{
    for (auto& iconName : iconNames)
    {
        uiManager.deactivateComponent(iconName);
    }
}

}