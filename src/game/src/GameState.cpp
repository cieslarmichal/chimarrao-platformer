#include "GameState.h"

#include "AnimatorFactory.h"
#include "GameStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "core/AnimationComponent.h"
#include "core/BoxColliderComponent.h"
#include "core/GraphicsComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "ui/DefaultUIManager.h"

namespace game
{

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::unique_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ComponentOwnersManager> componentOwnersManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    auto player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10, 10}, "player");
    auto graphicsComponent = player->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{5, 5}, utils::Vector2f{10, 10}, graphics::Color{128, 91, 50},
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>();
    auto animatorsFactory = animations::AnimatorFactory::createAnimatorFactory(rendererPool);
    std::shared_ptr<animations::Animator> bunnyAnimator = animatorsFactory->createBunnyAnimator(graphicsId);
    player->addComponent<components::core::AnimationComponent>(bunnyAnimator);
    player->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{5, 5});

    auto obstacle = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{30, 30}, "obstacle");
    obstacle->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{5, 5}, utils::Vector2f{30, 30},
        utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png",
        graphics::VisibilityLayer::Second);
    obstacle->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{5, 5});

    player->loadDependentComponents();
    obstacle->loadDependentComponents();
    timer.start();

    componentOwnersManager->add(player);
    componentOwnersManager->add(obstacle);
    componentOwnersManager->processNewObjects();
}

NextState GameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        componentOwnersManager->update(deltaTime, input);
        uiManager->update(deltaTime, input);
    }

    return NextState::Same;
}

void GameState::lateUpdate(const utils::DeltaTime&) {}

void GameState::render()
{
    rendererPool->renderAll();
}

StateType GameState::getType() const
{
    return StateType::Game;
}

void GameState::activate()
{
    active = true;
    paused = false;
    timer.restart();
    componentOwnersManager->activate();
    uiManager->activate();
}

void GameState::deactivate()
{
    active = false;
    timer.restart();
    componentOwnersManager->deactivate();
    uiManager->deactivate();
}

void GameState::pause()
{
    paused = true;
    componentOwnersManager->deactivate();
    states.addNextState(StateType::Pause);
}

}
