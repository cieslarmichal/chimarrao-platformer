#include "FriendFollowerComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

FriendFollowerComponent::FriendFollowerComponent(ComponentOwner* owner, ComponentOwner* player)
    : MovementComponent{owner, 6.5f}, player{player}
{
}

void FriendFollowerComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"FollowerComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"FriendFollowerComponent: Velocity component not found"};
    }
}

void FriendFollowerComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();
    currentMovementSpeed.x = 0;

    const auto distanceBetweenFriendAndPlayerOnXAxis =
        player->transform->getPosition().x - owner->transform->getPosition().x;

    if (distanceBetweenFriendAndPlayerOnXAxis < -8.f)
    {
        if (not canMoveLeft and not canMoveDown and canMoveUp)
        {
            currentMovementSpeed.y = -3.4f * 4.5f;
        }

        currentMovementSpeed.x = -maximumMovementSpeed;
    }
    else if (distanceBetweenFriendAndPlayerOnXAxis > 4.f)
    {
        if (not canMoveRight and not canMoveDown and canMoveUp)
        {
            currentMovementSpeed.y = -3.4f * 4.5f;
        }

        currentMovementSpeed.x = maximumMovementSpeed;
    }

    if (distanceBetweenFriendAndPlayerOnXAxis < -4)
    {
        animation->setAnimationDirection(animations::AnimationDirection::Left);
    }
    else
    {
        animation->setAnimationDirection(animations::AnimationDirection::Right);
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

void FriendFollowerComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
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