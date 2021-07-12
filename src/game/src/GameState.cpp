#include "GameState.h"

#include <utility>

#include "AnimatorFactory.h"
#include "GameStateUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"
#include "core/AnimationComponent.h"
#include "core/BoxColliderComponent.h"
#include "core/CameraComponent.h"
#include "core/FollowerComponent.h"
#include "core/GraphicsComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "core/VelocityComponent.h"
#include "ui/DefaultUIManager.h"

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

    auto player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10.f, 10.f}, "player");
    auto graphicsComponent = player->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{6.f, 3.75f}, utils::Vector2f{10.f, 10.f}, graphics::Color::White,
        graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>();
    auto animatorsFactory = animations::AnimatorFactory::createAnimatorFactory(rendererPool);
    const std::shared_ptr<animations::Animator> playerAnimator =
        animatorsFactory->createPlayerAnimator(graphicsId);
    player->addComponent<components::core::AnimationComponent>(playerAnimator);
    player->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 3.75f}, components::core::CollisionLayer::Player, utils::Vector2f{2.f, -0.1f});
    player->addComponent<components::core::VelocityComponent>();
    player->addComponent<components::core::CameraComponent>(
        rendererPool, utils::FloatRect{0, 0, tileMap->getSize().x * 4.f, tileMap->getSize().y * 4.f});

    auto enemy = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{2.f, 10.f}, "enemy");
    auto enemyGraphicsComponent = enemy->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{3.f, 3.f}, utils::Vector2f{2.f, 10.f}, graphics::Color::White,
        graphics::VisibilityLayer::Second);
    auto enemyGraphicsId = enemyGraphicsComponent->getGraphicsId();
    enemy->addComponent<components::core::FollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> bunnyAnimator =
        animatorsFactory->createBunnyAnimator(enemyGraphicsId);
    enemy->addComponent<components::core::AnimationComponent>(bunnyAnimator);
    enemy->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{3.f, 3.f}, components::core::CollisionLayer::Player, utils::Vector2f{0.f, 0.f});
    enemy->addComponent<components::core::VelocityComponent>();

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
    auto leftMapBorder =
        std::make_shared<components::core::ComponentOwner>(utils::Vector2f{-1, 0}, "left border");
    leftMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, tileMap->getSize().y * 4.f}, components::core::CollisionLayer::Default);
    auto topMapBorder =
        std::make_shared<components::core::ComponentOwner>(utils::Vector2f{0, -1}, "top border");
    topMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 1}, components::core::CollisionLayer::Default);
    auto rightMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 0}, "right border");
    rightMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1, tileMap->getSize().y * 4.f}, components::core::CollisionLayer::Default);
    auto bottomMapBorder = std::make_shared<components::core::ComponentOwner>(
        utils::Vector2f{0, tileMap->getSize().y * 4.f}, "bottom border");
    bottomMapBorder->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{tileMap->getSize().x * 4.f, 1}, components::core::CollisionLayer::Default);
    componentOwnersManager->add(leftMapBorder);
    componentOwnersManager->add(topMapBorder);
    componentOwnersManager->add(rightMapBorder);
    componentOwnersManager->add(bottomMapBorder);

    componentOwnersManager->add(player);
    componentOwnersManager->add(enemy);
    componentOwnersManager->processNewObjects();

    timer = utils::TimerFactory::createTimer();
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
