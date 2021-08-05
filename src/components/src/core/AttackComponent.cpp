#include "AttackComponent.h"

#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

AttackComponent::AttackComponent(ComponentOwner* owner, std::shared_ptr<physics::RayCast> rayCast)
    : Component{owner}, rayCast{std::move(rayCast)}

{
}

void AttackComponent::loadDependentComponents()
{
    directionComponent = owner->getComponent<DirectionComponent>();
    if (directionComponent)
    {
        directionComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"AttackComponent: DirectionComponent not found"};
    }

    animationComponent = owner->getComponent<AnimationComponent>();
    if (animationComponent)
    {
        animationComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"AttackComponent: AnimationComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (boxColliderComponent)
    {
        boxColliderComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"AttackComponent: BoxColliderComponent not found"};
    }
}

void AttackComponent::attack() const
{
    if (animationComponent->getAnimationType() == animations::AnimationType::Attack)
    {
        return;
    }

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

}