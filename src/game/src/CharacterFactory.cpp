#include "CharacterFactory.h"

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
#include "core/ItemCollectorComponent.h"
#include "core/KeyboardMovementComponent.h"
#include "core/VelocityComponent.h"

namespace game
{
CharacterFactory::CharacterFactory(std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                                   std::shared_ptr<TileMap> tileMapInit,
                                   std::shared_ptr<physics::DefaultRayCast> rayCastInit,
                                   std::shared_ptr<physics::DefaultQuadtree> quadtreeInit)
    : rendererPool{std::move(rendererPoolInit)},
      tileMap{std::move(tileMapInit)},
      rayCast{std::move(rayCastInit)},
      quadtree{std::move(quadtreeInit)},
      animatorFactory{animations::AnimatorFactory::createAnimatorFactory(rendererPool)}
{
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createPlayer(const utils::Vector2f& position)
{
    auto player = std::make_shared<components::core::ComponentOwner>(position, "player");
    auto graphicsComponent =
        player->addGraphicsComponent(rendererPool, utils::Vector2f{6.f, 3.75f}, position,
                                     graphics::Color::White, graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardMovementComponent>();
    const std::shared_ptr<animations::Animator> playerAnimator =
        animatorFactory->createPlayerAnimator(graphicsId);
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
    player->addComponent<components::core::ItemCollectorComponent>(quadtree, rayCast, 8);
    return player;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createRabbitFollower(const std::shared_ptr<components::core::ComponentOwner>& player,
                                       const utils::Vector2f& position)
{
    auto follower = std::make_shared<components::core::ComponentOwner>(position, "follower");
    auto followerGraphicsComponent =
        follower->addGraphicsComponent(rendererPool, utils::Vector2f{2.f, 2.f}, position,
                                       graphics::Color::White, graphics::VisibilityLayer::Second);
    auto followerGraphicsId = followerGraphicsComponent->getGraphicsId();
    follower->addComponent<components::core::FollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> bunnyAnimator =
        animatorFactory->createBunnyAnimator(followerGraphicsId);
    follower->addComponent<components::core::AnimationComponent>(bunnyAnimator);
    follower->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 2.f}, components::core::CollisionLayer::Player, utils::Vector2f{0.f, 0.f});
    follower->addComponent<components::core::VelocityComponent>();
    follower->addComponent<components::core::HealthComponent>(50);
    follower->addComponent<components::core::HealthBarComponent>(rendererPool, utils::Vector2f{0, -1});
    return follower;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createDruidNpc(const std::shared_ptr<components::core::ComponentOwner>& player,
                                 const utils::Vector2f& position)
{
    auto npc = std::make_shared<components::core::ComponentOwner>(position, "npc");
    auto npcGraphicsComponent =
        npc->addGraphicsComponent(rendererPool, utils::Vector2f{3.f, 3.5f}, position, graphics::Color::White,
                                  graphics::VisibilityLayer::Second);
    auto npcGraphicsId = npcGraphicsComponent->getGraphicsId();
    const std::shared_ptr<animations::Animator> druidAnimator =
        animatorFactory->createDruidAnimator(npcGraphicsId);
    npc->addComponent<components::core::AnimationComponent>(druidAnimator);
    npc->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1.6f, 3.5f}, components::core::CollisionLayer::Player, utils::Vector2f{0.6f, -0.1f});
    npc->addComponent<components::core::VelocityComponent>();
    npc->addComponent<components::core::IdleNpcMovementComponent>(player.get());
    return npc;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createBanditEnemy(const std::string& name,
                                    const std::shared_ptr<components::core::ComponentOwner>& player,
                                    const utils::Vector2f& position)
{
    auto enemy = std::make_shared<components::core::ComponentOwner>(position, name);
    auto enemyGraphicsComponent =
        enemy->addGraphicsComponent(rendererPool, utils::Vector2f{3.5f, 3.75f}, position,
                                    graphics::Color::White, graphics::VisibilityLayer::Second);
    auto enemyGraphicsId = enemyGraphicsComponent->getGraphicsId();
    enemy->addComponent<components::core::FollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> banditAnimator =
        animatorFactory->createBanditAnimator(enemyGraphicsId);
    enemy->addComponent<components::core::AnimationComponent>(banditAnimator);
    enemy->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 2.95f}, components::core::CollisionLayer::Player, utils::Vector2f{0.7f, 0.8f});
    enemy->addComponent<components::core::VelocityComponent>();
    enemy->addComponent<components::core::HealthComponent>(50);
    enemy->addComponent<components::core::HealthBarComponent>(rendererPool, utils::Vector2f{0.6, -1});
    return enemy;
}

}