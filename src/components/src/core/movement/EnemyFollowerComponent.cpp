#include "EnemyFollowerComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

EnemyFollowerComponent::EnemyFollowerComponent(ComponentOwner* owner, ComponentOwner* player)
    : MovementComponent{owner, 5.25f}, player{player}
{
}

void EnemyFollowerComponent::loadDependentComponents()
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
        throw exceptions::DependentComponentNotFound{"EnemyFollowerComponent: Velocity component not found"};
    }
}

void EnemyFollowerComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    const auto distanceBetweenEnemyAndPlayerOnXAxis =
        player->transform->getPosition().x - owner->transform->getPosition().x;

    if (distanceBetweenEnemyAndPlayerOnXAxis < -4.f and distanceBetweenEnemyAndPlayerOnXAxis > -45.f)
    {
        if (not canMoveLeft and not canMoveDown and canMoveUp)
        {
            currentMovementSpeed.y = -3.4f * 4.5f;
        }

        currentMovementSpeed.x = -maximumMovementSpeed;
    }
    else if (distanceBetweenEnemyAndPlayerOnXAxis > 2.f and distanceBetweenEnemyAndPlayerOnXAxis < 45.f)
    {
        if (not canMoveRight and not canMoveDown and canMoveUp)
        {
            currentMovementSpeed.y = -3.4f * 4.5f;
        }

        currentMovementSpeed.x = maximumMovementSpeed;
    }

    if (distanceBetweenEnemyAndPlayerOnXAxis < 0)
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

void EnemyFollowerComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
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