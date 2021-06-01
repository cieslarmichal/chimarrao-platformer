#pragma once

#include "DeltaTime.h"
#include "Input.h"
#include "Timer.h"
#include "ui/UIManager.h"

namespace game
{
struct GridButtonInfo
{
    std::string buttonName;
    unsigned iconIndex;
    bool horizontalMoveCauseAction;
};

struct ButtonIndexHash
{
    std::size_t operator()(const utils::Vector2u& vec) const
    {
        return std::hash<std::string>()(toString(vec));
    };
};

class GridButtonsNavigator
{
public:
    explicit GridButtonsNavigator(components::ui::UIManager&,
                                  const std::vector<std::vector<GridButtonInfo>>& gridButtonsInfo,
                                  const std::vector<std::string>& iconNames,
                                  graphics::Color buttonsDefaultColor, graphics::Color buttonsHoverColor);

    void update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void setFocusOnItem(const utils::Vector2u& itemIndex);
    void loseFocus();

private:
    std::unordered_set<utils::Vector2u, ButtonIndexHash> getAvailableGridButtonsIndices();
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButtonLeft();
    void changeSelectedButtonRight();
    void changeSelectedButton(const utils::Vector2u& buttonIndex);
    void unselectAllButtons();
    void setIconAssociatedWithButtonVisible(const utils::Vector2u& buttonIndex);
    void hideIcons();

    components::ui::UIManager& uiManager;
    const std::vector<std::vector<GridButtonInfo>> gridButtonsInfo;
    const std::unordered_set<utils::Vector2u, ButtonIndexHash> availableGridButtonsIndices;
    const std::vector<std::string> iconNames;
    utils::Vector2u currentItemIndex{0, 0};
    utils::Timer switchItemTimer;
    const float timeAfterButtonCanBeSwitched;
    const graphics::Color buttonsDefaultColor;
    const graphics::Color buttonsHoverColor;
};
}