#include "DistanceAttack.h"

#include "exceptions/DependentComponentNotFound.h"
#include "health/HealthComponent.h"

namespace components::core
{

DistanceAttack::DistanceAttack(ComponentOwner* owner,
                               const std::shared_ptr<components::core::SharedContext>& sharedContextInit,
                               std::shared_ptr<components::core::ComponentOwnersManager> ownersManagerInit)
    : owner{owner},
      sharedContext{sharedContextInit},
      ownersManager{std::move(ownersManagerInit)},
      animatorFactory{animations::AnimatorFactory::createAnimatorFactory(sharedContext->rendererPool)}
{
}

void DistanceAttack::attack()
{
    std::call_once(componentsInitialized, [this] { loadDependentComponents(); });

    const auto heading = directionComponent->getHeading();
    const auto& ownerPosition = boxColliderComponent->getPosition();
    const auto size = boxColliderComponent->getSize();

    const auto startPositionOnXAxis = heading.x == 1 ? ownerPosition.x + size.x + 1 : ownerPosition.x - 1;
    const auto startPositionOnYAxis = ownerPosition.y + (size.y / 2.f);
    const auto startPosition = utils::Vector2f{startPositionOnXAxis, startPositionOnYAxis};

    static int numberOfProjectilesInGame = 0;
    numberOfProjectilesInGame++;
    projectile = std::make_shared<components::core::ComponentOwner>(
        startPosition, "projectile" + std::to_string(numberOfProjectilesInGame), sharedContext,
        components::core::ComponentOwnerType::Friend);
    auto projectileGraphicsComponent = projectile->addGraphicsComponent(
        sharedContext->rendererPool, utils::Vector2f{3.f, 3.5f}, startPosition, graphics::Color::White,
        graphics::VisibilityLayer::Second);
    auto projectileGraphicsId = projectileGraphicsComponent->getGraphicsId();
    const std::shared_ptr<animations::Animator> druidAnimator =
        animatorFactory->createDruidAnimator(projectileGraphicsId);
    projectile->addComponent<components::core::AnimationComponent>(druidAnimator);
    projectile->addComponent<components::core::BoxColliderComponent>(
        utils::Vector2f{1.6f, 3.5f}, components::core::CollisionLayer::Player, utils::Vector2f{0.6f, -0.1f});
    projectile->addComponent<components::core::VelocityComponent>(0);
    //    projectile->addComponent<components::core::IdleNpcMovementComponent>(player.get());

    ownersManager->add(projectile);
}

void DistanceAttack::loadDependentComponents()
{
    directionComponent = owner->getComponent<DirectionComponent>();
    if (not directionComponent)
    {
        throw exceptions::DependentComponentNotFound{"DistanceAttack: DirectionComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (not boxColliderComponent)
    {
        throw exceptions::DependentComponentNotFound{"DistanceAttack: BoxColliderComponent not found"};
    }
}
}