#pragma once

#include "State.h"
#include "Timer.h"
#include "Window.h"
#include "core/ComponentOwner.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class MenuStateUIConfigBuilder;

class MenuState : public State
{
public:
    friend class MenuStateUIConfigBuilder;

    explicit MenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                       std::shared_ptr<utils::FileAccess>, States&,
                       std::unique_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    unsigned int currentButtonIndex;
    utils::Timer switchButtonTimer;
    const float timeAfterButtonCanBeSwitched;
    const std::vector<std::string> buttonNames;
    std::unique_ptr<components::ui::UIManager> uiManager;
    bool shouldExit;
    std::vector<std::string> iconUniqueNames;
};
}