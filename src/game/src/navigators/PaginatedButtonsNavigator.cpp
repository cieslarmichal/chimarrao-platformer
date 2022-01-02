#include "PaginatedButtonsNavigator.h"

#include <cmath>
#include <utility>

namespace game
{

PaginatedButtonsNavigator::PaginatedButtonsNavigator(
    std::shared_ptr<components::ui::UIManager> uiManager, const std::vector<std::string>& uniqueButtonNames,
    const std::vector<std::string>& iconNames, const std::vector<std::string>& itemsToPaginate,
    std::function<void(int)> paginatedIndexAction, std::size_t maximumNumberOfItemsToDisplay,
    graphics::Color buttonsDefaultColor, graphics::Color buttonsHoverColor,
    std::unique_ptr<utils::Timer> moveTimer, std::unique_ptr<utils::Timer> actionTimer)
    : uiManager{std::move(uiManager)},
      uniqueButtonNames{uniqueButtonNames},
      iconNames{iconNames},
      itemsToPaginate{itemsToPaginate},
      paginatedIndexAction{std::move(paginatedIndexAction)},
      buttonNamesWithIndices{getButtonNamesWithIndices()},
      buttonsDefaultColor{buttonsDefaultColor},
      buttonsHoverColor{buttonsHoverColor},
      timeAfterButtonCanBeSwitched{0.1f},
      timeAfterActionCanBeExecuted{0.1f},
      switchButtonTimer{std::move(moveTimer)},
      actionTimer{std::move(actionTimer)},
      maximumNumberOfItemsToDisplay{maximumNumberOfItemsToDisplay},
      currentPage{1},
      numberOfPages{static_cast<unsigned int>(std::ceil(static_cast<float>(itemsToPaginate.size()) /
                                                        static_cast<float>(maximumNumberOfItemsToDisplay)))}
{
    if (uniqueButtonNames.size() != iconNames.size())
    {
        throw std::runtime_error{"Icons size is not equal buttons size"};
    }

    if (numberOfPages == 0)
    {
        throw std::runtime_error{"No items to paginate"};
    }
}

void PaginatedButtonsNavigator::initialize()
{
    uiManager->setColor(uniqueButtonNames.at(currentItemIndex), buttonsHoverColor);
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
            changePageToLeft();
        }
        else if (input.isKeyPressed(input::InputKey::Right))
        {
            changePageToRight();
        }

        switchButtonTimer->restart();
    }

    if (actionTimer->getElapsedSeconds() > timeAfterActionCanBeExecuted)
    {
        if (input.isKeyPressed(input::InputKey::Enter))
        {
            uiManager->invokeClickAction(uniqueButtonNames.at(currentItemIndex), input::InputKey::MouseLeft);
        }
        else if (input.isKeyPressed(input::InputKey::Escape))
        {
            return NextState::Previous;
        }

        actionTimer->restart();
    }

    return NextState::Same;
}

void PaginatedButtonsNavigator::changePageToRight()
{
    if (currentPage < numberOfPages)
    {
        const auto numberOfItemsRemaining =
            itemsToPaginate.size() - currentPage * maximumNumberOfItemsToDisplay;

        for (std::size_t itemIndex = 0;
             itemIndex < numberOfItemsRemaining && itemIndex < maximumNumberOfItemsToDisplay; itemIndex++)
        {
            const auto paginatedItemIndex = currentPage * maximumNumberOfItemsToDisplay + itemIndex;
            const auto& paginatedItem = itemsToPaginate[paginatedItemIndex];
            uiManager->setText(uniqueButtonNames[itemIndex], paginatedItem);
            const auto paginatedIndexActionCopy = paginatedIndexAction;
            auto overriddenAction = [paginatedIndexActionCopy, paginatedItemIndex]
            { paginatedIndexActionCopy(paginatedItemIndex); };
            components::core::KeyAction loadMapKeyAction{input::InputKey::MouseLeft, overriddenAction};
            uiManager->changeClickAction(uniqueButtonNames[itemIndex], {loadMapKeyAction});
        }

        if (numberOfItemsRemaining < maximumNumberOfItemsToDisplay)
        {
            for (auto itemIndex = numberOfItemsRemaining; itemIndex < maximumNumberOfItemsToDisplay;
                 itemIndex++)
            {
                uiManager->deactivateComponent(uniqueButtonNames[itemIndex]);
            }
        }
        setFocusOnButton(uniqueButtonNames.front());
        currentPage++;
    }
}

void PaginatedButtonsNavigator::changePageToLeft()
{
    if (currentPage > 1)
    {
        for (std::size_t itemIndex = 0; itemIndex < maximumNumberOfItemsToDisplay; itemIndex++)
        {
            const auto paginatedItemIndex = (currentPage - 2) * maximumNumberOfItemsToDisplay + itemIndex;
            const auto& paginatedItem = itemsToPaginate[paginatedItemIndex];
            uiManager->activateComponent(uniqueButtonNames[itemIndex]);
            uiManager->setText(uniqueButtonNames[itemIndex], paginatedItem);
            const auto paginatedIndexActionCopy = paginatedIndexAction;
            auto overriddenAction = [paginatedIndexActionCopy, paginatedItemIndex]
            { paginatedIndexActionCopy(paginatedItemIndex); };
            components::core::KeyAction loadMapKeyAction{input::InputKey::MouseLeft, overriddenAction};
            uiManager->changeClickAction(uniqueButtonNames[itemIndex], {loadMapKeyAction});
        }
        setFocusOnButton(uniqueButtonNames.front());
        currentPage--;
    }
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
        uiManager->setColor(buttonName, buttonsHoverColor);
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

    for (std::size_t index = 0; index < uniqueButtonNames.size(); ++index)
    {
        buttonNamesWithIndicesInit.insert({uniqueButtonNames[index], index});
    }
    return buttonNamesWithIndicesInit;
}

void PaginatedButtonsNavigator::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentItemIndex == 0)
    {
        currentItemIndex = static_cast<unsigned int>(uniqueButtonNames.size() - 1);
    }
    else
    {
        if (currentItemIndex == uniqueButtonNames.size() - 1)
        {
            const auto numberOfItemsOnPage = getNumberOfItemsOnPage();
            currentItemIndex = numberOfItemsOnPage - 1;
        }
        else
        {
            --currentItemIndex;
        }
    }

    uiManager->setColor(uniqueButtonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentItemIndex == uniqueButtonNames.size() - 1)
    {
        currentItemIndex = 0;
    }
    else
    {
        if (const auto numberOfItemsOnPage = getNumberOfItemsOnPage();
            (currentItemIndex + 1) % maximumNumberOfItemsToDisplay >= numberOfItemsOnPage)
        {
            currentItemIndex = uniqueButtonNames.size() - 1;
        }
        else
        {
            ++currentItemIndex;
        }
    }
    uiManager->setColor(uniqueButtonNames.at(currentItemIndex), buttonsHoverColor);
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::changeSelectedButton(unsigned int buttonIndex)
{
    currentItemIndex = buttonIndex;
    setIconVisible(currentItemIndex);
}

void PaginatedButtonsNavigator::unselectAllButtons()
{
    for (const auto& buttonName : uniqueButtonNames)
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

unsigned PaginatedButtonsNavigator::getNumberOfItemsOnPage() const
{
    if (static_cast<int>(itemsToPaginate.size() / (currentPage * maximumNumberOfItemsToDisplay)) > 0)
    {
        return maximumNumberOfItemsToDisplay;
    }

    return itemsToPaginate.size() % maximumNumberOfItemsToDisplay;
}

}