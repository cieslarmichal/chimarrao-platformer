#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "Window.h"
#include "core/ComponentOwner.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class MenuStateUIConfigBuilder;

class MenuState : public State, public input::InputObserver
{
public:
    friend class MenuStateUIConfigBuilder;

    explicit MenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                       const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&,
                       std::unique_ptr<components::ui::UIManager>);
    ~MenuState();

    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void handleButtonSwitching();
    void createIcons();
    void addIcon(const utils::Vector2f& position);
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    const input::InputStatus* inputStatus;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> icons;
    unsigned int currentButtonIndex;
    utils::Timer switchButtonTimer;
    const float timeAfterButtonCanBeSwitched;
    const std::vector<std::string> buttonNames;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}