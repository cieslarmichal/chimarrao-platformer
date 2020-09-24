#pragma once

#include "State.h"
#include "Window.h"

namespace game
{
class MainMenuState : public State
{
public:
    explicit MainMenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
};
}