#include "GameState.h"

#include <utility>

#include "AnimatorFactory.h"
#include "GameStateUIConfigBuilder.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "ProjectPathReader.h"
#include "TimerFactory.h"
#include "core/AnimationComponent.h"
#include "core/AttackComponent.h"
#include "core/BoxColliderComponent.h"
#include "core/CameraComponent.h"
#include "core/DirectionComponent.h"
#include "core/FollowerComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HealthBarComponent.h"
#include "core/HealthComponent.h"
#include "core/IdleNpcMovementComponent.h"
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
                     std::shared_ptr<TileMap> tileMapInit, std::shared_ptr<physics::RayCast> rayCastInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      uiManager{std::move(uiManagerInit)},
      componentOwnersManager{std::move(componentOwnersManagerInit)},
      tileMap{std::move(tileMapInit)},
      rayCast{std::move(rayCastInit)}
{
    uiManager->createUI(GameStateUIConfigBuilder::createGameUIConfig(this));

    auto player = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{10.f, 10.f}, "player");
    auto graphicsComponent =
        player->addGraphicsComponent(rendererPool, utils::Vector2f{6.f, 3.75f}, utils::Vector2f{10.f, 10.f},
                                     graphics::Color::White, graphics::VisibilityLayer::Second);
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
    player->addComponent<components::core::HealthComponent>(1000);
    player->addComponent<components::core::DirectionComponent>();
    player->addComponent<components::core::AttackComponent>(rayCast);
    player->addComponent<components::core::HealthBarComponent>(rendererPool, utils::Vector2f{1.5, -1});

    hud = std::make_unique<HeadsUpDisplay>(player, rendererPool,
                                           HeadsUpDisplayUIConfigBuilder::createUIConfig());

    auto follower = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{2.f, 10.f}, "follower");
    auto followerGraphicsComponent =
        follower->addGraphicsComponent(rendererPool, utils::Vector2f{2.f, 2.f}, utils::Vector2f{2.f, 10.f},
                                    graphics::Color::White, graphics::VisibilityLayer::Second);
    auto followerGraphicsId = followerGraphicsComponent->getGraphicsId();
    follower->addComponent<components::core::FollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> bunnyAnimator =
        animatorsFactory->createBunnyAnimator(followerGraphicsId);
    follower->addComponent<components::core::AnimationComponent>(bunnyAnimator);
    follower->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 2.f}, components::core::CollisionLayer::Player, utils::Vector2f{0.f, 0.f});
    follower->addComponent<components::core::VelocityComponent>();
    follower->addComponent<components::core::HealthComponent>(50);
    follower->addComponent<components::core::HealthBarComponent>(rendererPool, utils::Vector2f{0, -1});

    auto npc = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{35.f, 10.f}, "npc");
    auto npcGraphicsComponent =
        npc->addGraphicsComponent(rendererPool, utils::Vector2f{3.f, 3.5f}, utils::Vector2f{35.f, 10.f},
                                  graphics::Color::White, graphics::VisibilityLayer::Second);
    auto npcGraphicsId = npcGraphicsComponent->getGraphicsId();
    const std::shared_ptr<animations::Animator> druidAnimator =
        animatorsFactory->createDruidAnimator(npcGraphicsId);
    npc->addComponent<components::core::AnimationComponent>(druidAnimator);
    npc->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1.6f, 3.5f}, components::core::CollisionLayer::Player, utils::Vector2f{0.6f, -0.1f});
    npc->addComponent<components::core::VelocityComponent>();
    npc->addComponent<components::core::IdleNpcMovementComponent>(player.get());

    auto enemy = std::make_shared<components::core::ComponentOwner>(utils::Vector2f{40.f, 10.f}, "enemy");
    auto enemyGraphicsComponent =
        enemy->addGraphicsComponent(rendererPool, utils::Vector2f{3.5f, 3.75f}, utils::Vector2f{40.f, 10.f},
                                       graphics::Color::White, graphics::VisibilityLayer::Second);
    auto enemyGraphicsId = enemyGraphicsComponent->getGraphicsId();
    enemy->addComponent<components::core::FollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> banditAnimator =
        animatorsFactory->createBanditAnimator(enemyGraphicsId);
    enemy->addComponent<components::core::AnimationComponent>(banditAnimator);
    enemy->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 2.95f}, components::core::CollisionLayer::Player, utils::Vector2f{0.7f, 0.8f});
    enemy->addComponent<components::core::VelocityComponent>();
    enemy->addComponent<components::core::HealthComponent>(50);
    enemy->addComponent<components::core::HealthBarComponent>(rendererPool, utils::Vector2f{0.6, -1});

    for (int x = 0; x < tileMap->getSize().x; x++)
    {
        for (int y = 0; y < tileMap->getSize().y; y++)
        {
            if (tileMap->getTile(utils::Vector2i{x, y})->type)
            {
                auto obstacle = std::make_shared<components::core::ComponentOwner>(
                    utils::Vector2f{x * 4.f, y * 4.f},
                    "obstacle" + std::to_string(x) + std::to_string(y) + std::to_string(y));
                obstacle->addGraphicsComponent(
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
    componentOwnersManager->add(follower);
    componentOwnersManager->add(npc);
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
        hud->update();
    }

    componentOwnersManager->processRemovals();

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
