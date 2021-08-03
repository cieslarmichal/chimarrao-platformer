#include "DirectionComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

DirectionComponent::DirectionComponent(ComponentOwner* owner) : Component{owner} {}

void DirectionComponent::loadDependentComponents()
{
    velocityComponent = owner->getComponent<VelocityComponent>();
    if (not velocityComponent)
    {
        throw exceptions::DependentComponentNotFound{"DirectionComponent: VelocityComponent not found"};
    }
}

animations::AnimationDirection DirectionComponent::getDirection() const
{
    const auto currentVelocity = velocityComponent->getVelocity();
    if (currentVelocity.x >= 0)
    {
        return animations::AnimationDirection::Right;
    }

    return animations::AnimationDirection::Left;
}
}