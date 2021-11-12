#include "MeleeAttack.h"

#include <utility>

#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

MeleeAttack::MeleeAttack(ComponentOwner* owner, std::shared_ptr<physics::RayCast> rayCast)
    : owner{owner}, rayCast{std::move(rayCast)}
{
}

void MeleeAttack::attack()
{
    std::call_once(componentsInitialized, [this] { loadDependentComponents();});

    const auto heading = directionComponent->getHeading();
    const auto& ownerPosition = owner->transform->getPosition();
    const auto size = boxColliderComponent->getSize();

    const auto startPoint =
        utils::Vector2f{ownerPosition.x + (size.x / 2.f), ownerPosition.y + (size.y / 2.f)};
    const auto endPoint =
        utils::Vector2f{startPoint.x + (static_cast<float>(heading.x) * range), startPoint.y};

    const auto result = rayCast->cast(startPoint, endPoint, owner->getId(), 2);

    if (result.collision)
    {
        if (auto health = result.collision->getComponent<HealthComponent>())
        {
            health->loseHealthPoints(damage);
        }
    }
}

void MeleeAttack::loadDependentComponents()
{
    directionComponent = owner->getComponent<DirectionComponent>();
    if (not directionComponent)
    {
        throw exceptions::DependentComponentNotFound{"MeleeAttack: DirectionComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (not boxColliderComponent)
    {
        throw exceptions::DependentComponentNotFound{"MeleeAttack: BoxColliderComponent not found"};
    }
}
}