#include "MainGameState.h"

#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include "KeyboardMovementComponent.h"
#include "animation/AnimatorSettingsYamlReader.h"
#include "animation/DefaultAnimatorSettingsRepository.h"
#include "animation/PlayerAnimator.h"

namespace game
{

MainGameState::MainGameState(const std::shared_ptr<graphics::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit)
    : GameState{windowInit, inputManagerInit, rendererPoolInit}
{
    graphics::animation::DefaultAnimatorSettingsRepository settingsRepository{
        std::make_unique<graphics::animation::AnimatorSettingsYamlReader>()};

    player = std::make_shared<components::ComponentOwner>(utils::Vector2f{10, 10});
    auto graphicsComponent = player->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{5, 5}, utils::Vector2f{10, 10});
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::KeyboardMovementComponent>(inputManager);
    auto playerAnimatorSettings = settingsRepository.getAnimatorSettings("player");
    auto playerAnimator = std::make_shared<graphics::animation::PlayerAnimator>(graphicsId, rendererPool,
                                                                                *playerAnimatorSettings);
    player->addComponent<components::AnimationComponent>(playerAnimator);

    initialize();
}

void MainGameState::initialize()
{
    player->loadDependentComponents();
    player->start();
}

void MainGameState::update(const utils::DeltaTime& deltaTime)
{
    player->update(deltaTime);
}

void MainGameState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    player->lateUpdate(deltaTime);
}

void MainGameState::render()
{
    rendererPool->renderAll();
}

void MainGameState::checkIfEnded() {}

}
