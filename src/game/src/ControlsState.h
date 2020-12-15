#pragma once

#include "ComponentOwner.h"
#include "InputObserver.h"
#include "State.h"
#include "Timer.h"

namespace game
{
class ControlsState : public State, public input::InputObserver
{
public:
    explicit ControlsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&,
                           std::stack<std::unique_ptr<State>>&);
    ~ControlsState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void unfreezeButtons();
    void backToMenu();
    void createBackground();
    void createControlsTitle();
    void createBackToMenuButton();
    void createControlButtons();
    void createDescriptionsForControlButtons();
    void addNonClickableButton(const utils::Vector2f& position, const utils::Vector2f& size,
                               const std::string& text, unsigned int fontSize,
                               const utils::Vector2f& textOffset);
    void addButtonWithMouseOver(const utils::Vector2f& position, const utils::Vector2f& size,
                                const std::string& text, unsigned int fontSize,
                                const utils::Vector2f& textOffset, std::function<void(void)> clickAction);
    void addText(const utils::Vector2f& position, const std::string& description, unsigned int fontSize);

    bool shouldBackToMenu;
    const input::InputStatus* inputStatus;
    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::unique_ptr<components::ComponentOwner>> texts;
    std::vector<std::unique_ptr<components::ComponentOwner>> buttons;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}