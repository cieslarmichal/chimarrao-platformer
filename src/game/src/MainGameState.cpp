#include "MainGameState.h"

#include "GetProjectPath.h"
#include "animation/PlayerAnimator.h"

namespace game
{

MainGameState::MainGameState(std::shared_ptr<graphics::Window> window, InputManager& inputManagerInit,
                             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             std::shared_ptr<physics::PhysicsEngine> physicsEngineInit)
    : GameState{std::move(window), inputManagerInit, std::move(rendererPoolInit),
                std::move(physicsEngineInit)}
{
    auto graphicsId = rendererPool->acquire({5, 5}, {10, 10}, utils::getProjectPath("chimarrao") + "xxx.png");
    auto physicsId = physicsEngine->acquire({5, 5}, {10, 10});

    player = std::make_unique<Player>(
        graphicsId, rendererPool, physicsId, physicsEngine,
        std::make_unique<graphics::animation::PlayerAnimator>(graphicsId, rendererPool));

    auto* playerAsObserver = dynamic_cast<Player*>(player.get());
    if (playerAsObserver)
    {
        inputManager.registerObserver(playerAsObserver);
    }
}

void MainGameState::update(const utils::DeltaTime& dt)
{
    player->update(dt);
    physicsEngine->update(dt);
}

void MainGameState::render()
{
    rendererPool->renderAll();
}

void MainGameState::checkIfEnded() {}

}
