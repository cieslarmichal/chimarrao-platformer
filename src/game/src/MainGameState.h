#pragma once

#include "GameState.h"
#include "Player.h"
#include "Window.h"

namespace game
{
class MainGameState : public GameState
{
public:
    explicit MainGameState(std::shared_ptr<graphics::Window>, input::InputManager&,
                           std::shared_ptr<graphics::RendererPool>, std::shared_ptr<physics::PhysicsEngine>);

    void update(const utils::DeltaTime&) override;
    void render() override;
    void checkIfEnded() override;

private:
    std::unique_ptr<Entity> player;
};
}