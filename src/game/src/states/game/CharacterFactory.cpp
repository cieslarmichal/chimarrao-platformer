#include "CharacterFactory.h"

#include <utility>

#include "AnimationComponent.h"
#include "ArtificialIntelligenceAttackComponent.h"
#include "BoxColliderComponent.h"
#include "CameraComponent.h"
#include "CommonUIConfigElements.h"
#include "DialogueTextComponent.h"
#include "DirectionComponent.h"
#include "DistanceAttack.h"
#include "EnemyFollowerComponent.h"
#include "FriendFollowerComponent.h"
#include "GraphicsComponent.h"
#include "IdleNpcMovementComponent.h"
#include "ItemCollectorComponent.h"
#include "KeyboardAnimatedMovementComponent.h"
#include "KeyboardDistanceAttackComponent.h"
#include "KeyboardMeleeAttackComponent.h"
#include "LimitedSpaceActionComponent.h"
#include "MeleeAttack.h"
#include "TimerFactory.h"
#include "VelocityComponent.h"
#include "health/HealthBarComponent.h"
#include "health/HealthComponent.h"

namespace game
{
CharacterFactory::CharacterFactory(
    const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
    std::shared_ptr<TileMap> tileMapInit, std::shared_ptr<physics::RayCast> rayCastInit,
    std::shared_ptr<physics::Quadtree> quadtreeInit,
    std::shared_ptr<components::core::ComponentOwnersManager> ownersManagerInit)
    : sharedContext{sharedContextInit},
      tileMap{std::move(tileMapInit)},
      rayCast{std::move(rayCastInit)},
      quadtree{std::move(quadtreeInit)},
      animatorFactory{animations::AnimatorFactory::createAnimatorFactory(sharedContext->rendererPool)},
      ownersManager{std::move(ownersManagerInit)}
{
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createPlayer(const utils::Vector2f& position, std::function<void(void)> deadAction)
{
    auto player = std::make_shared<components::core::ComponentOwner>(
        position, "player", sharedContext, components::core::ComponentOwnerType::Player);
    auto graphicsComponent =
        player->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{6.f, 3.75f}, position,
                                     graphics::Color::White, graphics::VisibilityLayer::Second);
    auto graphicsId = graphicsComponent->getGraphicsId();
    player->addComponent<components::core::KeyboardAnimatedMovementComponent>();
    const std::shared_ptr<animations::Animator> playerAnimator =
        animatorFactory->createPlayerAnimator(graphicsId);
    player->addComponent<components::core::AnimationComponent>(playerAnimator);
    player->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 3.75f}, components::core::CollisionLayer::Player, utils::Vector2f{2.f, -0.1f});
    player->addComponent<components::core::VelocityComponent>(8);
    player->addComponent<components::core::CameraComponent>(
        sharedContext->rendererPool, utils::FloatRect{0, 0, static_cast<float>(tileMap->getSize().x) * 4.f,
                                                      static_cast<float>(tileMap->getSize().y) * 4.f});
    player->addComponent<components::core::HealthComponent>(200, std::move(deadAction));
    player->addComponent<components::core::DirectionComponent>();
    auto friendlyFireValidator = std::make_unique<components::core::DefaultFriendlyFireValidator>();
    auto meleeAttack = std::make_shared<components::core::MeleeAttack>(player.get(), rayCast,
                                                                       std::move(friendlyFireValidator));
    player->addComponent<components::core::KeyboardMeleeAttackComponent>(meleeAttack);
    auto distanceAttack =
        std::make_shared<components::core::DistanceAttack>(player.get(), sharedContext, ownersManager);
    player->addComponent<components::core::KeyboardDistanceAttackComponent>(distanceAttack);
    const std::shared_ptr<utils::Timer> itemCollectorTimer = utils::TimerFactory::createTimer();
    player->addComponent<components::core::ItemCollectorComponent>(quadtree, rayCast, 8, itemCollectorTimer);
    player->addComponent<components::core::DialogueTextComponent>(sharedContext->rendererPool, position, "",
                                                                  fontPath, 9, -0.3f, graphics::Color::Black,
                                                                  utils::Vector2f{0, -2});
    return player;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createRabbitFollower(const std::shared_ptr<components::core::ComponentOwner>& player,
                                       const utils::Vector2f& position)
{
    auto follower = std::make_shared<components::core::ComponentOwner>(
        position, "follower", sharedContext, components::core::ComponentOwnerType::Friend);
    auto followerGraphicsComponent =
        follower->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{1.5f, 1.5f}, position,
                                       graphics::Color::White, graphics::VisibilityLayer::Second);
    auto followerGraphicsId = followerGraphicsComponent->getGraphicsId();
    follower->addComponent<components::core::FriendFollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> bunnyAnimator =
        animatorFactory->createBunnyAnimator(followerGraphicsId);
    follower->addComponent<components::core::AnimationComponent>(bunnyAnimator);
    follower->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1.5f, 1.5f}, components::core::CollisionLayer::Player, utils::Vector2f{0.f, 0.f});
    follower->addComponent<components::core::VelocityComponent>(6.5);
    follower->addComponent<components::core::HealthComponent>(50);
    follower->addComponent<components::core::DialogueTextComponent>(sharedContext->rendererPool, position, "",
                                                                    fontPath, 9, -2.2, graphics::Color::Black,
                                                                    utils::Vector2f{0, -3.6});
    return follower;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createDruidNpc(const std::shared_ptr<components::core::ComponentOwner>& player,
                                 const utils::Vector2f& position, std::function<void(void)> interaction)
{
    static int numberOfDruidsInGame = 0;
    numberOfDruidsInGame++;
    auto npc = std::make_shared<components::core::ComponentOwner>(
        position, "npcDruid" + std::to_string(numberOfDruidsInGame), sharedContext,
        components::core::ComponentOwnerType::Friend);
    auto npcGraphicsComponent =
        npc->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{3.f, 3.5f}, position,
                                  graphics::Color::White, graphics::VisibilityLayer::Second);
    auto npcGraphicsId = npcGraphicsComponent->getGraphicsId();
    const std::shared_ptr<animations::Animator> druidAnimator =
        animatorFactory->createDruidAnimator(npcGraphicsId);
    npc->addComponent<components::core::AnimationComponent>(druidAnimator);
    npc->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1.6f, 3.5f}, components::core::CollisionLayer::Player, utils::Vector2f{0.6f, -0.1f});
    npc->addComponent<components::core::VelocityComponent>(0);
    npc->addComponent<components::core::IdleNpcMovementComponent>(player.get());
    npc->addComponent<components::core::DialogueTextComponent>(
        sharedContext->rendererPool, position, "Press E to talk", fontPath, 9, 0, graphics::Color::Black,
        utils::Vector2f{0, -2});
    npc->addComponent<components::core::LimitedSpaceActionComponent>(player.get(), std::move(interaction),
                                                                     10);
    return npc;
}

std::shared_ptr<components::core::ComponentOwner>
CharacterFactory::createBanditEnemy(const std::shared_ptr<components::core::ComponentOwner>& player,
                                    const utils::Vector2f& position)
{
    static int numberOfBanditsInGame = 0;
    numberOfBanditsInGame++;
    auto enemy = std::make_shared<components::core::ComponentOwner>(
        position, "bandit" + std::to_string(numberOfBanditsInGame), sharedContext,
        components::core::ComponentOwnerType::Enemy);
    auto enemyGraphicsComponent =
        enemy->addGraphicsComponent(sharedContext->rendererPool, utils::Vector2f{3.5f, 3.75f}, position,
                                    graphics::Color::White, graphics::VisibilityLayer::Second);
    auto enemyGraphicsId = enemyGraphicsComponent->getGraphicsId();
    enemy->addComponent<components::core::EnemyFollowerComponent>(player.get());
    const std::shared_ptr<animations::Animator> banditAnimator =
        animatorFactory->createBanditAnimator(enemyGraphicsId);
    enemy->addComponent<components::core::AnimationComponent>(banditAnimator);
    enemy->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{2.f, 2.95f}, components::core::CollisionLayer::Player, utils::Vector2f{0.7f, 0.8f});
    enemy->addComponent<components::core::VelocityComponent>(5.5);
    enemy->addComponent<components::core::DirectionComponent>();
    auto friendlyFireValidator = std::make_unique<components::core::DefaultFriendlyFireValidator>();
    auto attackStrategy = std::make_shared<components::core::MeleeAttack>(enemy.get(), rayCast,
                                                                          std::move(friendlyFireValidator));
    enemy->addComponent<components::core::ArtificialIntelligenceAttackComponent>(player.get(),
                                                                                 attackStrategy);
    enemy->addComponent<components::core::HealthComponent>(50);
    enemy->addComponent<components::core::HealthBarComponent>(sharedContext->rendererPool,
                                                              utils::Vector2f{1.f, -0.5});
    return enemy;
}

}