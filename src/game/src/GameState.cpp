#include "GameState.h"

#include "AnimationComponent.h"
#include "AnimatorSettingsYamlReader.h"
#include "DefaultAnimatorSettingsRepository.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "KeyboardMovementComponent.h"
#include "PlayerAnimator.h"
#include "TextComponent.h"

namespace game
{

GameState::GameState(const std::shared_ptr<gui::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit)
    : State{windowInit, inputManagerInit, rendererPoolInit}
{
    animations::DefaultAnimatorSettingsRepository settingsRepository{
        std::make_unique<animations::AnimatorSettingsYamlReader>()};

    player = std::make_shared<components::ComponentOwner>(utils::Vector2f{10, 10});
    auto graphicsComponent = player->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{50, 50}, utils::Vector2f{10, 10});
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::KeyboardMovementComponent>(inputManager);
    auto playerAnimatorSettings = settingsRepository.getAnimatorSettings("player");
    auto playerAnimator =
        std::make_shared<animations::PlayerAnimator>(graphicsId, rendererPool, *playerAnimatorSettings);
    player->addComponent<components::AnimationComponent>(playerAnimator);
    player->addComponent<components::TextComponent>(rendererPool, utils::Vector2f{10, 10}, "hello",
                                                    utils::getProjectPath("chimarrao-platformer") +
                                                        "resources/fonts/VeraMono.ttf",
                                                    12, graphics::Color::Black);

    initialize();
}

void GameState::initialize()
{
    player->loadDependentComponents();
    player->start();
}

void GameState::update(const utils::DeltaTime& deltaTime)
{
    player->update(deltaTime);
}

void GameState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    player->lateUpdate(deltaTime);
}

void GameState::render()
{
    rendererPool->renderAll();
}

}
