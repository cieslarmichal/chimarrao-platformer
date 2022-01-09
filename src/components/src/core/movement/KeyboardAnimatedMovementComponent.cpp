#include "KeyboardAnimatedMovementComponent.h"

#include "AnimationComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardAnimatedMovementComponent::KeyboardAnimatedMovementComponent(ComponentOwner* ownerInit)
    : MovementComponent{ownerInit}
{
}

void KeyboardAnimatedMovementComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Velocity component not found"};
    }
}

void KeyboardAnimatedMovementComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled or locked)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();
    auto maximumMovementSpeed = velocityComponent->getMaxMovementSpeed();

    bool running = false;

    if (input.isKeyPressed(input::InputKey::Left) &&
        (animation->getAnimationDirection() != animations::AnimationDirection::Right ||
         animation->getAnimationType() != animations::AnimationType::Roll))
    {
        animation->setAnimationDirection(animations::AnimationDirection::Left);
        if (input.isKeyPressed(input::InputKey::Down) ||
            animation->getAnimationType() == animations::AnimationType::Roll)
        {
            animation->setAnimation(animations::AnimationType::Roll);
            currentMovementSpeed.x = -1.5f * maximumMovementSpeed;
        }
        else
        {
            if (animation->getAnimationType() != animations::AnimationType::Roll)
            {
                if (input.isKeyPressed(input::InputKey::Shift))
                {
                    currentMovementSpeed.x = -1.7f * maximumMovementSpeed;
                    running = true;
                }
                else
                {
                    currentMovementSpeed.x = -maximumMovementSpeed;
                }
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
            currentMovementSpeed.x = 1.5f * maximumMovementSpeed;
        }
        else
        {
            if (animation->getAnimationType() != animations::AnimationType::Roll)
            {
                if (input.isKeyPressed(input::InputKey::Shift))
                {
                    currentMovementSpeed.x = 1.7f * maximumMovementSpeed;
                    running = true;
                }
                else
                {
                    currentMovementSpeed.x = maximumMovementSpeed;
                }
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
        else if (running)
        {
            animation->setAnimation(animations::AnimationType::Run);
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

    velocityComponent->setVelocity(currentMovementSpeed);
}

void KeyboardAnimatedMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
{
    if (not enabled or locked)
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