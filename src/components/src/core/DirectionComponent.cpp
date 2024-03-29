#include "DirectionComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

DirectionComponent::DirectionComponent(ComponentOwner* owner) : Component{owner} {}

void DirectionComponent::loadDependentComponents()
{
    animationComponent = owner->getComponent<AnimationComponent>();
    if (animationComponent)
    {
        animationComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"DirectionComponent: AnimationComponent not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"DirectionComponent: VelocityComponent not found"};
    }
}

animations::AnimationDirection DirectionComponent::getDirection() const
{
    const auto currentVelocity = velocityComponent->getVelocity();
    if (currentVelocity.x > 0)
    {
        return animations::AnimationDirection::Right;
    }
    else if (currentVelocity.x < 0)
    {
        return animations::AnimationDirection::Left;
    }

    return animationComponent->getAnimationDirection();
}

sf::Vector2i DirectionComponent::getHeading() const
{
    const auto direction = getDirection();

    sf::Vector2i heading{1, 1};

    if (direction == animations::AnimationDirection::Right)
    {
        heading.x = 1;
    }
    else
    {
        heading.x = -1;
    }

    return heading;
}
}