#include "MainGameState.h"

#include "GetProjectPath.h"
#include "animation/AnimatorSettingsYamlReader.h"
#include "animation/PlayerAnimator.h"

namespace game
{

MainGameState::MainGameState(std::shared_ptr<graphics::Window> window, InputManager& inputManagerInit,
                             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             std::shared_ptr<physics::PhysicsEngine> physicsEngineInit)
    : GameState{std::move(window), inputManagerInit, std::move(rendererPoolInit),
                std::move(physicsEngineInit)}
{
    auto graphicsId = rendererPool->acquire({5, 5}, {10, 10},
                                            utils::getProjectPath("chimarrao") +
                                                "resources/Player/Idle/idle-with-weapon-1.png");
    auto physicsId = physicsEngine->acquire({5, 5}, {10, 10});

    auto animatorsSettings = graphics::animation::AnimatorSettingsYamlReader::readAnimatorsSettings(
        utils::getProjectPath("chimarrao") + "config/animators.yaml");

    // TODO: add animatorsSettings base for all animators from vector to concrete animator
    // TODO: clean up
    if (animatorsSettings)
    {
        auto playerAnimatorSettings =
            std::find_if(animatorsSettings->cbegin(), animatorsSettings->cend(),
                         [](const graphics::animation::AnimatorSettings& animatorSettings) {
                             return animatorSettings.animatorName == "player";
                         });

        if (playerAnimatorSettings != animatorsSettings->cend())
        {
            player = std::make_unique<Player>(graphicsId, rendererPool, physicsId, physicsEngine,
                                              std::make_unique<graphics::animation::PlayerAnimator>(
                                                  graphicsId, rendererPool, *playerAnimatorSettings));

            auto* playerAsObserver = dynamic_cast<Player*>(player.get());
            if (playerAsObserver)
            {
                inputManager.registerObserver(playerAsObserver);
            }
        }
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
