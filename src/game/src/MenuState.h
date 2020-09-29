#pragma once

#include "ComponentOwner.h"
#include "State.h"
#include "Window.h"

namespace game
{
class MenuState : public State
{
public:
    explicit MenuState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&,
                           std::stack<std::unique_ptr<State>>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;

private:
    void createBackground();
    void createPlayGameButton();
    void createMapEditorButton();
    void createSettingsButton();
    void createExitButton();
    void addButton(const utils::Vector2f& position, const std::string& text,
                   const utils::Vector2f& textOffset, std::function<void(void)> clickAction);

    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::unique_ptr<components::ComponentOwner>> buttons;
};
}