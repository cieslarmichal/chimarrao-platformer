#include "IdleNpcMovementComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

IdleNpcMovementComponent::IdleNpcMovementComponent(ComponentOwner* owner, ComponentOwner* player)
    : MovementComponent{owner}, player{player}
{
}

void IdleNpcMovementComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "IdleNpcMovementComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "IdleNpcMovementComponent: Velocity component not found"};
    }
}

void IdleNpcMovementComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (player->transform->getPosition().x >= owner->transform->getPosition().x)
    {
        animation->setAnimationDirection(animations::AnimationDirection::Right);
    }
    else
    {
        animation->setAnimationDirection(animations::AnimationDirection::Left);
    }

    if (canMoveDown)
    {
        currentMovementSpeed.y += 25.f * deltaTime.count();
    }
    else
    {
        animation->setAnimation(animations::AnimationType::Idle);
    }

    velocityComponent->setVelocity(currentMovementSpeed);
}

void IdleNpcMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
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