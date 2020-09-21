#pragma once

#include "Window.h"
#include "GameState.h"

namespace game
{
class MainMenuState : public GameState
{
public:
    explicit MainMenuState(const std::shared_ptr<gui::Window>&, const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    void checkIfEnded() override;
};
}