#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "Window.h"
#include "core/ComponentOwner.h"

namespace game
{
class MenuState : public State, public input::InputObserver
{
public:
    explicit MenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                       const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&);
    ~MenuState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void handleButtonSwitching();
    void unfreezeButtons();
    void createBackground();
    void createPlayGameButton();
    void createMapEditorButton();
    void createControlsButton();
    void createSettingsButton();
    void createExitButton();
    void addButton(const utils::Vector2f& position, const std::string& text,
                   const utils::Vector2f& textOffset, std::function<void(void)> clickAction);
    void createIcons();
    void addIcon(const utils::Vector2f& position);
    void changeSelectedButtonUp();
    void changeSelectedButtonDown();
    void changeSelectedButton(unsigned int buttonIndex);
    void unselectAllButtons();
    void setIconVisible(unsigned int iconIndex);
    void hideIcons();

    const input::InputStatus* inputStatus;
    std::unique_ptr<components::core::ComponentOwner> background;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> buttons;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> icons;
    unsigned int currentButtonIndex;
    utils::Timer switchButtonTimer;
    const float timeAfterButtonCanBeSwitched;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}