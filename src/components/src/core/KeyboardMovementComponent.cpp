#include "KeyboardMovementComponent.h"

#include "AnimationComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardMovementComponent::KeyboardMovementComponent(ComponentOwner* ownerInit)
    : Component{ownerInit}, movementSpeed{6.f}
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

    if (input.isKeyPressed(input::InputKey::Left) &&
        (animation->getAnimationDirection() != animations::AnimationDirection::Right ||
         animation->getAnimationType() != animations::AnimationType::Roll))
    {
        animation->setAnimationDirection(animations::AnimationDirection::Left);
        if (input.isKeyPressed(input::InputKey::Down) ||
            animation->getAnimationType() == animations::AnimationType::Roll)
        {
            animation->setAnimation(animations::AnimationType::Roll);
            currentMovementSpeed.x = -1.5f * movementSpeed;
        }
        else
        {
            if (animation->getAnimationType() != animations::AnimationType::Roll)
            {
                currentMovementSpeed.x = -movementSpeed;
            }
        }
    }
    else if (input.isKeyPressed(input::InputKey::Right) &&
             (animation->getAnimationDirection() != animations::AnimationDirection::Left ||
              animation->getAnimationType() != animations::AnimationType::Roll))
    {
        animation->setAnimationDirection(animations::AnimationDirection::Right);
        if (input.isKeyPressed(input::InputKey::Down) ||
            animation->getAnimationType() == animations::AnimationType::Roll)
        {
            animation->setAnimation(animations::AnimationType::Roll);
            currentMovementSpeed.x = 1.5f * movementSpeed;
        }
        else
        {
            if (animation->getAnimationType() != animations::AnimationType::Roll)
            {
                currentMovementSpeed.x = movementSpeed;
            }
        }
    }
    else
    {
        if (animation->getAnimationType() != animations::AnimationType::Roll)
        {
            currentMovementSpeed.x = 0.f;
        }
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

    if (input.isKeyPressed(input::InputKey::Up) && not canMoveDown && canMoveUp &&
        animation->getAnimationType() != animations::AnimationType::Roll)
    {
        currentMovementSpeed.y = -3.4f * 5.f;
        animation->setAnimation(animations::AnimationType::Jump);
    }
    else
    {
        currentMovementSpeed.y += 25.f * deltaTime.count();
    }

    if (input.isKeyPressed(input::InputKey::Space))
    {
        animation->setAnimation(animations::AnimationType::Attack);
    }

    velocityComponent->setVelocity(currentMovementSpeed);
}

void KeyboardMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
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

void KeyboardMovementComponent::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}

float KeyboardMovementComponent::getMovementSpeed() const
{
    return movementSpeed;
}
}