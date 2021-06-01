#pragma once

#include "DeltaTime.h"
#include "Input.h"
#include "Timer.h"
#include "ui/UIManager.h"

namespace game
{
class StateUINavigator
{
public:
    explicit StateUINavigator(components::ui::UIManager&, const std::vector<std::string>& buttonNames,
                              const std::vector<std::string>& iconNames);

    void update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void setFocusOnItem(unsigned int itemIndex);
    void loseFocus();

private:
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    components::ui::UIManager& uiManager;
    const std::vector<std::string> buttonNames;
    const std::vector<std::string> iconNames;
    unsigned int currentItemIndex{0};
    utils::Timer switchItemTimer;
    const float timeAfterButtonCanBeSwitched;
};
}