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

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit}
{
    animations::DefaultAnimatorSettingsRepository settingsRepository{
        std::make_unique<animations::AnimatorSettingsYamlReader>()};

    player = std::make_shared<components::ComponentOwner>(utils::Vector2f{10, 10});
    auto graphicsComponent = player->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{7, 7}, utils::Vector2f{10, 10}, graphics::Color::Red,
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::KeyboardMovementComponent>(inputManager);
    auto playerAnimatorSettings = settingsRepository.getAnimatorSettings("player");
    auto playerAnimator =
        std::make_shared<animations::PlayerAnimator>(graphicsId, rendererPool, *playerAnimatorSettings);
    player->addComponent<components::AnimationComponent>(playerAnimator);
    player->addComponent<components::TextComponent>(rendererPool, utils::Vector2f{10, 10}, "hello",
                                                    utils::getProjectPath("chimarrao-platformer") +
                                                        "resources/fonts/VeraMono.ttf",
                                                    13, graphics::Color::Black, utils::Vector2f{1.5, -1.5});

    background = std::make_shared<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png",
        graphics::VisibilityLayer::Background);
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

std::string GameState::getName() const
{
    return "Game state";
}

}
