#pragma once

#include "NextState.h"
#include "Timer.h"
#include "UIManager.h"

namespace game
{

class PaginatedButtonsNavigator
{
public:
    explicit PaginatedButtonsNavigator(
        std::shared_ptr<components::ui::UIManager>, const std::vector<std::string>& uniqueButtonNames,
        const std::vector<std::string>& iconNames, const std::vector<std::string>& itemsToPaginate,
        std::function<void(int)> paginatedIndexAction, std::size_t maximumNumberOfItemsToDisplay,
        graphics::Color buttonsDefaultColor, graphics::Color buttonsHoverColor,
        std::unique_ptr<utils::Timer> moveTimer, std::unique_ptr<utils::Timer> actionTimer);

    void initialize();
    NextState update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void setFocusOnButton(const std::string& buttonName);
    void loseFocus();
    void changePageToRight();
    void changePageToLeft();

private:
    std::unordered_map<std::string, unsigned> getButtonNamesWithIndices();
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();
    unsigned getNumberOfItemsOnPage() const;

    std::shared_ptr<components::ui::UIManager> uiManager;
    const std::vector<std::string> uniqueButtonNames;
    const std::vector<std::string> iconNames;
    std::vector<std::string> itemsToPaginate;
    std::function<void(int)> paginatedIndexAction;
    const std::unordered_map<std::string, unsigned> buttonNamesWithIndices;
    unsigned int currentItemIndex{0};
    const graphics::Color buttonsDefaultColor;
    const graphics::Color buttonsHoverColor;
    const float timeAfterButtonCanBeSwitched;
    const float timeAfterActionCanBeExecuted;
    std::unique_ptr<utils::Timer> switchButtonTimer;
    std::unique_ptr<utils::Timer> actionTimer;
    const std::size_t maximumNumberOfItemsToDisplay;
    unsigned int currentPage;
    unsigned int numberOfPages;
};
}