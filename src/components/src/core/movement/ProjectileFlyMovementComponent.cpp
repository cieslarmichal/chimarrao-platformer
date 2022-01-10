#include "ProjectileFlyMovementComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

ProjectileFlyMovementComponent::ProjectileFlyMovementComponent(ComponentOwner* ownerInit,
                                                               animations::AnimationDirection direction)
    : MovementComponent{ownerInit}, direction{direction}
{
}

void ProjectileFlyMovementComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ProjectileFlyMovementComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ProjectileFlyMovementComponent: Velocity component not found"};
    }
}

void ProjectileFlyMovementComponent::update(utils::DeltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();
    auto maximumMovementSpeed = velocityComponent->getMaxMovementSpeed();

    if (direction == animations::AnimationDirection::Left)
    {
        currentMovementSpeed.x = -maximumMovementSpeed;
    }

    if (direction == animations::AnimationDirection::Right)
    {
        currentMovementSpeed.x = maximumMovementSpeed;
    }

    velocityComponent->setVelocity(currentMovementSpeed);
}

void ProjectileFlyMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (currentMovementSpeed.x < 0)
    {
        if (canMoveLeft)
        {
            animation->setAnimation(animations::AnimationType::Fly, animations::AnimationDirection::Left);
        }
        else
        {
            currentMovementSpeed.x = 0;
        }
    }

    if (currentMovementSpeed.x > 0)
    {
        if (canMoveRight)
        {
            animation->setAnimation(animations::AnimationType::Fly, animations::AnimationDirection::Right);
        }
        else
        {
            currentMovementSpeed.x = 0;
        }
    }

    velocityComponent->setVelocity(currentMovementSpeed);
    const float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    const float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}
}