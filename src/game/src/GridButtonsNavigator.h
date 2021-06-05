#pragma once

#include "ButtonsNavigator.h"
#include "GridButtonInfo.h"
#include "NextState.h"
#include "Timer.h"
#include "ui/UIManager.h"

namespace game
{
class GridButtonsNavigator : public ButtonsNavigator
{
public:
    explicit GridButtonsNavigator(std::shared_ptr<components::ui::UIManager>,
                                  const std::vector<std::vector<GridButtonInfo>>& gridButtonsInfo,
                                  const std::vector<std::string>& iconNames,
                                  graphics::Color buttonsDefaultColor, graphics::Color buttonsHoverColor,
                                  std::unique_ptr<utils::Timer> moveTimer,
                                  std::unique_ptr<utils::Timer> actionTimer);

    void initialize() override;
    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void activate() override;
    void setFocusOnButton(const std::string& buttonName) override;
    void loseFocus() override;

private:
    std::unordered_map<std::string, utils::Vector2u> getButtonNamesWithIndices();
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButtonLeft();
    void changeSelectedButtonRight();
    void changeSelectedButton(const utils::Vector2u& buttonIndex);
    void unselectAllButtons();
    void setIconAssociatedWithButtonVisible(const utils::Vector2u& buttonIndex);
    void hideIcons();

    std::shared_ptr<components::ui::UIManager> uiManager;
    const std::vector<std::vector<GridButtonInfo>> gridButtonsInfo;
    const std::unordered_map<std::string, utils::Vector2u> buttonNamesWithIndices;
    const std::vector<std::string> iconNames;
    utils::Vector2u currentButtonIndex{0, 0};
    const graphics::Color buttonsDefaultColor;
    const graphics::Color buttonsHoverColor;
    const float timeAfterButtonCanBeSwitched;
    const float timeAfterActionCanBeExecuted;
    std::unique_ptr<utils::Timer> switchButtonTimer;
    std::unique_ptr<utils::Timer> actionTimer;
};
}