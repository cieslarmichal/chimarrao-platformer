#include "MainGameState.h"

#include "PhysicsSfmlComponent.h"

namespace game
{

MainGameState::MainGameState(std::shared_ptr<graphics::Window> window, InputManager& inputManagerInit)
    : GameState{std::move(window), inputManagerInit}
{
    player = std::make_unique<Player>(
        std::make_unique<physics::PhysicsSfmlComponent>(utils::Vector2f{50, 50}, utils::Vector2f{50, 50}));

    auto* playerAsObserver = dynamic_cast<Player*>(player.get());
    if (playerAsObserver)
    {
        inputManager.registerObserver(playerAsObserver);
    }
}

void MainGameState::update(const utils::DeltaTime& dt)
{
    player->update(dt);
}

void MainGameState::render(sf::RenderTarget* target)
{
    player->render(target);
}

void MainGameState::checkIfEnded() {}

}
