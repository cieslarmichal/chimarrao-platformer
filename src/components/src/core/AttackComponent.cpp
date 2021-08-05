#include "AttackComponent.h"

#include "exceptions/DependentComponentNotFound.h"

#include "HealthComponent.h"

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
    const auto heading = directionComponent->getHeading();
    const auto& startPoint = owner->transform->getPosition();
    const auto size = boxColliderComponent->getSize();

    sf::Vector2f endPoint;

    endPoint.x = startPoint.x + (size.x/2.f) + (static_cast<float>(heading.x) * range);
    endPoint.y = startPoint.y + (size.y/2.f);

    const auto result = rayCast->cast(startPoint, endPoint, owner->getId(), 2);

    if (result.collision)
    {
        auto health =  result.collision->getComponent<HealthComponent>();
        health->loseHealthPoints(damage);
    }
}

}