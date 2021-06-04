#include "GameState.h"

#include <utility>

#include "AnimatorFactory.h"
#include "GameStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "core/AnimationComponent.h"
#include "core/BoxColliderComponent.h"
#include "core/GraphicsComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "core/VelocityComponent.h"
#include "ui/DefaultUIManager.h"
#include "TimerFactory.h"

namespace game
{

GameState::GameState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                     std::shared_ptr<components::ui::UIManager> uiManagerInit,
                     std::unique_ptr<ComponentOwnersManager> componentOwnersManagerInit,
                     std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)},
      tileMap{std::move(tileMapInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    auto player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10, 10}, "player");
    auto graphicsComponent = player->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{3.8, 3.8}, utils::Vector2f{10, 10}, graphics::Color::White,
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>();
    auto animatorsFactory = animations::AnimatorFactory::createAnimatorFactory(rendererPool);
    std::shared_ptr<animations::Animator> druidAnimator = animatorsFactory->createPlayerAnimator(graphicsId);
    player->addComponent<components::core::AnimationComponent>(druidAnimator);
    player->addComponent<components::core::BoxColliderComponent>(utils::Vector2f{3.75, 3.75},
                                                                 components::core::CollisionLayer::Player);
    player->addComponent<components::core::VelocityComponent>();

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            if (tileMap->getTile(utils::Vector2i{x, y})->type)
            {
                auto obstacle = std::make_shared<components::core::ComponentOwner>(
                    utils::Vector2f{x * 4.f, y * 4.f},
                    "obstacle" + std::to_string(x) + std::to_string(y) + std::to_string(y));
                obstacle->addComponent<components::core::GraphicsComponent>(
                    rendererPool, utils::Vector2f{4, 4}, utils::Vector2f{x * 4.f, y * 4.f},
                    tileTypeToPathTexture(*(tileMap->getTile(utils::Vector2i{x, y})->type)),
                    graphics::VisibilityLayer::Second);
                obstacle->addComponent<components::core::BoxColliderComponent>(
                    utils::Vector2f{4, 4}, components::core::CollisionLayer::Tile);
                componentOwnersManager->add(obstacle);
            }
        }
    }

    timer = utils::TimerFactory::createTimer();

    componentOwnersManager->add(player);
    componentOwnersManager->processNewObjects();
}

NextState GameState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (timer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
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

void GameState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

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
    timer->restart();
    componentOwnersManager->activate();
    uiManager->activate();
}

void GameState::deactivate()
{
    active = false;
    timer->restart();
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
