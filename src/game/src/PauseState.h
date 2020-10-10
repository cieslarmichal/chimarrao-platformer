#pragma once

#include "ComponentOwner.h"
#include "State.h"
#include "InputObserver.h"

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
    void createBackground();
    void createBackToGameButton();
    void createMenuButton();
    void addButton(const utils::Vector2f& position, const std::string& text,
                   const utils::Vector2f& textOffset, std::function<void(void)> clickAction);

    const input::InputStatus* inputStatus;
    std::function<void(void)> backToGameCallback;
    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::unique_ptr<components::ComponentOwner>> buttons;
};
}