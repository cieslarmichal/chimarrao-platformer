#include "KeyboardMovementComponent.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardMovementComponent::KeyboardMovementComponent(ComponentOwner* ownerInit)
    : Component{ownerInit}, movementSpeed{5.f}
{
}

void KeyboardMovementComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (not animation)
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (not velocityComponent)
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Velocity component not found"};
    }
}

void KeyboardMovementComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (input.isKeyPressed(input::InputKey::Left) && canMoveLeft)
    {

        currentMovementSpeed.x = -movementSpeed;

        animation->setAnimationDirection(animations::AnimationDirection::Left);
    }
    else if (input.isKeyPressed(input::InputKey::Right) && canMoveRight)
    {
        currentMovementSpeed.x = movementSpeed;
        animation->setAnimationDirection(animations::AnimationDirection::Right);
    }
    else
    {
        currentMovementSpeed.x = 0;
    }

    if (input.isKeyPressed(input::InputKey::Up) && canMoveUp && not canMoveDown)
    {
        currentMovementSpeed.y = -2.5f * movementSpeed;
    }
    else
    {
        if (not canMoveUp && currentMovementSpeed.y < 0.f)
        {
            currentMovementSpeed.y = 0.f;
        }
        else if (canMoveDown)
        {
            currentMovementSpeed.y += 15.f * deltaTime.count();
        }
        else
        {
            currentMovementSpeed.y = 0;
        }
    }

    if (currentMovementSpeed.x == 0 && currentMovementSpeed.y == 0)
    {
        animation->setAnimation(animations::AnimationType::Idle);
    }
    else
    {
        animation->setAnimation(animations::AnimationType::Walk);
    }

    velocityComponent->setVelocity(currentMovementSpeed);
    float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}

void KeyboardMovementComponent::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}

float KeyboardMovementComponent::getMovementSpeed() const
{
    return movementSpeed;
}

}