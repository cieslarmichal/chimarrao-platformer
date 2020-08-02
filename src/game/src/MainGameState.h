#pragma once

#include "GameState.h"
#include "Window.h"
#include "Player.h"

namespace game
{
class MainGameState : public GameState
{
public:
    explicit MainGameState(std::shared_ptr<graphics::Window>, InputManager&, std::shared_ptr<graphics::RendererPool>);

    void update(const utils::DeltaTime&) override;
    void render() override;
    void checkIfEnded() override;

private:
    std::unique_ptr<Entity> player;
};
}