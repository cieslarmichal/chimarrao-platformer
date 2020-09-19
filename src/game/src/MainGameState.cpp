#include "MainGameState.h"

#include "AnimationComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "KeyboardMovementComponent.h"
#include "animation/AnimatorSettingsYamlReader.h"
#include "animation/PlayerAnimator.h"

namespace game
{

MainGameState::MainGameState(const std::shared_ptr<graphics::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit)
    : GameState{windowInit, inputManagerInit, rendererPoolInit}
{
    auto animatorsSettings = graphics::animation::AnimatorSettingsYamlReader::readAnimatorsSettings(
        utils::getProjectPath("chimarrao-platformer") + "config/animators.yaml");

    // TODO: add animatorsSettings base for all animators from vector to concrete animator
    if (animatorsSettings)
    {
        auto playerAnimatorSettings =
            std::find_if(animatorsSettings->cbegin(), animatorsSettings->cend(),
                         [](const graphics::animation::AnimatorSettings& animatorSettings) {
                             return animatorSettings.animatorName == "player";
                         });

        if (playerAnimatorSettings != animatorsSettings->cend())
        {
            player = std::make_shared<components::ComponentOwner>(utils::Vector2f{10, 10});
            auto graphicsComponent = player->addComponent<components::GraphicsComponent>(
                rendererPool, utils::Vector2f{5, 5}, utils::Vector2f{10, 10});
            auto graphicsId = graphicsComponent->getGraphicsId();
            player->addComponent<components::KeyboardMovementComponent>(inputManager);
            auto playerAnimator = std::make_shared<graphics::animation::PlayerAnimator>(
                graphicsId, rendererPool, *playerAnimatorSettings);
            player->addComponent<components::AnimationComponent>(playerAnimator);
        }
    }

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
