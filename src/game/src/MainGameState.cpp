#include "MainGameState.h"

#include "PhysicsSfmlComponent.h"

namespace game
{

MainGameState::MainGameState(std::shared_ptr<graphics::Window> window, InputManager& inputManagerInit, std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : GameState{std::move(window), inputManagerInit, std::move(rendererPoolInit)}
{
    auto graphicsId = rendererPool->acquire({5,5}, {10, 10}, graphics::Color::Magenta);

    player = std::make_unique<Player>(graphicsId, rendererPool);

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

void MainGameState::render()
{
    rendererPool->renderAll();
}

void MainGameState::checkIfEnded() {}

}
