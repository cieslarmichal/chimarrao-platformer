#include "FollowerComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

FollowerComponent::FollowerComponent(ComponentOwner* owner, ComponentOwner* followedOwner)
    : MovementComponent{owner, 4.5f}, followedOwner{followedOwner}
{
}

void FollowerComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (not animation)
    {
        throw exceptions::DependentComponentNotFound{"FollowerComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (not velocityComponent)
    {
        throw exceptions::DependentComponentNotFound{"FollowerComponent: Velocity component not found"};
    }
}

void FollowerComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (followedOwner->transform->getPosition().x - owner->transform->getPosition().x < -2.f)
    {
        animation->setAnimationDirection(animations::AnimationDirection::Left);
        currentMovementSpeed.x = -movementSpeed;
    }
    else if (followedOwner->transform->getPosition().x - owner->transform->getPosition().x > 2.f)
    {
        animation->setAnimationDirection(animations::AnimationDirection::Right);
        currentMovementSpeed.x = movementSpeed;
    }

    if (not canMoveDown)
    {
        if (currentMovementSpeed.x == 0.f)
        {
            animation->setAnimation(animations::AnimationType::Idle);
        }
        else
        {
            animation->setAnimation(animations::AnimationType::Walk);
        }
    }

    if (canMoveDown)
    {
        currentMovementSpeed.y += 25.f * deltaTime.count();
    }

    velocityComponent->setVelocity(currentMovementSpeed);
}

void FollowerComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (currentMovementSpeed.x < 0 and not canMoveLeft)
    {
        currentMovementSpeed.x = 0;
    }
    if (currentMovementSpeed.x > 0 and not canMoveRight)
    {
        currentMovementSpeed.x = 0;
    }
    if (currentMovementSpeed.y < 0 and not canMoveUp)
    {
        currentMovementSpeed.y = 0;
    }
    if (currentMovementSpeed.y > 0 and not canMoveDown)
    {
        currentMovementSpeed.y = 0;
    }

    velocityComponent->setVelocity(currentMovementSpeed);
    const float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    const float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}

}