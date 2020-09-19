#include "KeyboardMovementComponent.h"
#include "ComponentOwner.h"
#include "AnimationComponent.h"

namespace components
{

KeyboardMovementComponent::KeyboardMovementComponent(ComponentOwner* owner, const std::shared_ptr<input::InputManager>& inputManager)
    : Component{owner}, movementSpeed{10.f}
{
    inputManager->registerObserver(this);
}

void KeyboardMovementComponent::awake()
{
    animation = owner->getComponent<AnimationComponent>();
}

void KeyboardMovementComponent::update(utils::DeltaTime deltaTime)
{
    float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}

void KeyboardMovementComponent::handleInputStatus(const input::InputStatus& inputStatus)
{
    currentMovementSpeed.x = 0;
    if (inputStatus.isKeyPressed(input::InputKey::Left))
    {
        currentMovementSpeed.x = -movementSpeed;
        animation->setAnimationDirection(graphics::animation::AnimationDirection::Left);
    }
    else if (inputStatus.isKeyPressed(input::InputKey::Right))
    {
        currentMovementSpeed.x = movementSpeed;
        animation->setAnimationDirection(graphics::animation::AnimationDirection::Right);
    }

    currentMovementSpeed.y = 0;
    if (inputStatus.isKeyPressed(input::InputKey::Up))
    {
        currentMovementSpeed.y = -movementSpeed;
    }
    else if (inputStatus.isKeyPressed(input::InputKey::Down))
    {
        currentMovementSpeed.y = movementSpeed;
    }

    if (currentMovementSpeed.x == 0 && currentMovementSpeed.y == 0)
    {
        animation->setAnimation(graphics::animation::AnimationType::Idle);
    }
    else
    {
        animation->setAnimation(graphics::animation::AnimationType::Walk);
    }
}

void KeyboardMovementComponent::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}

}