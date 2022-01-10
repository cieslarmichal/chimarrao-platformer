#include "DistanceAttack.h"

#include "exceptions/DependentComponentNotFound.h"
#include "health/HealthComponent.h"

namespace components::core
{

DistanceAttack::DistanceAttack(ComponentOwner* owner, std::shared_ptr<physics::RayCast> rayCast,
                         std::unique_ptr<FriendlyFireValidator> friendlyFireValidatorInit)
    : owner{owner}, rayCast{std::move(rayCast)}, friendlyFireValidator{std::move(friendlyFireValidatorInit)}
{
}

void DistanceAttack::attack()
{
    std::call_once(componentsInitialized, [this] { loadDependentComponents(); });

    const auto heading = directionComponent->getHeading();
    const auto& ownerPosition = boxColliderComponent->getPosition();
    const auto size = boxColliderComponent->getSize();

    const auto startPoint =
        utils::Vector2f{ownerPosition.x + (size.x / 2.f), ownerPosition.y + (size.y / 2.f)};
    const auto endPoint =
        utils::Vector2f{startPoint.x + (static_cast<float>(heading.x) * range), startPoint.y};

    const auto result = rayCast->cast(startPoint, endPoint, owner->getId(), 2);

    if (result.collision)
    {
        if (friendlyFireValidator->validate(owner, result.collision) ==
            FriendlyFireValidationResult::AttackNotAllowed)
        {
            return;
        }

        if (auto health = result.collision->getComponent<HealthComponent>())
        {
            health->loseHealthPoints(damage);
        }
    }
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