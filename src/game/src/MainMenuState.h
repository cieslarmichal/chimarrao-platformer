#pragma once

#include "State.h"
#include "Window.h"
#include "ComponentOwner.h"

namespace game
{
class MainMenuState : public State
{
public:
    explicit MainMenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;

private:
    std::shared_ptr<components::ComponentOwner> background;
    std::vector<std::shared_ptr<components::ComponentOwner>> buttons;
};
}