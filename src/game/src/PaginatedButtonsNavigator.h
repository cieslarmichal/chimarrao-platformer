#pragma once

#include "ButtonsNavigator.h"
#include "Timer.h"
#include "ui/UIManager.h"
#include "NextState.h"

namespace game
{

class PaginatedButtonsNavigator : public ButtonsNavigator
{
public:
    explicit PaginatedButtonsNavigator(std::shared_ptr<components::ui::UIManager>,
                                       const std::vector<std::string>& buttonNames,
                                       const std::vector<std::string>& iconNames,
                                       graphics::Color buttonsDefaultColor,
                                       graphics::Color buttonsHoverColor,
                                       std::unique_ptr<utils::Timer>);

    void initialize() override;
    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void activate() override;
    void setFocusOnButton(const std::string& buttonName) override;
    void loseFocus() override;

private:
    std::unordered_map<std::string, unsigned> getButtonNamesWithIndices();
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    std::shared_ptr<components::ui::UIManager> uiManager;
    const std::vector<std::string> buttonNames;
    const std::unordered_map<std::string, unsigned> buttonNamesWithIndices;
    const std::vector<std::string> iconNames;
    unsigned int currentItemIndex{0};
    const graphics::Color buttonsDefaultColor;
    const graphics::Color buttonsHoverColor;
    const float timeAfterButtonCanBeSwitched;
    std::unique_ptr<utils::Timer> switchItemTimer;
};
}