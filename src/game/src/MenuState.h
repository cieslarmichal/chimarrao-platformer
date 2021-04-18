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
                       States&, std::unique_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void createIcons();
    void addIcon(const utils::Vector2f& position);
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    std::vector<std::unique_ptr<components::core::ComponentOwner>> icons;
    unsigned int currentButtonIndex;
    utils::Timer switchButtonTimer;
    const float timeAfterButtonCanBeSwitched;
    const std::vector<std::string> buttonNames;
    std::unique_ptr<components::ui::UIManager> uiManager;
    bool shouldExit;
};
}