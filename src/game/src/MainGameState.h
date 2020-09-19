#pragma once

#include "ComponentOwner.h"
#include "GameState.h"
#include "Window.h"

namespace game
{
class MainGameState : public GameState
{
public:
    explicit MainGameState(const std::shared_ptr<graphics::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    void checkIfEnded() override;

private:
    std::shared_ptr<components::ComponentOwner> player;
};
}