#pragma once

#include "ComponentOwner.h"
#include "State.h"
#include "InputObserver.h"
#include "Timer.h"

namespace game
{
class PauseState : public State, public input::InputObserver
{
public:
    explicit PauseState(const std::shared_ptr<window::Window>&,
                       const std::shared_ptr<input::InputManager>&,
                       const std::shared_ptr<graphics::RendererPool>&,
                       std::stack<std::unique_ptr<State>>&);
    ~PauseState();

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
    void backToGame();
    void backToMenu();
    void createPauseTitle();
    void createBackground();
    void createBackToGameButton();
    void createMenuButton();
    void addButton(const utils::Vector2f& position, const std::string& text,
                   const utils::Vector2f& textOffset, std::function<void(void)> clickAction);

    const input::InputStatus* inputStatus;
    utils::Timer timer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToGame;
    bool shouldBackToMenu;
    std::unique_ptr<components::ComponentOwner> title;
    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::unique_ptr<components::ComponentOwner>> buttons;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}