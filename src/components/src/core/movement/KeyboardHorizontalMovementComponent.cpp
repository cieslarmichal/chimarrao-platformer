#include "KeyboardHorizontalMovementComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardHorizontalMovementComponent::KeyboardHorizontalMovementComponent(ComponentOwner* ownerInit)
    : MovementComponent{ownerInit}
{
}

void KeyboardHorizontalMovementComponent::loadDependentComponents()
{
    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardHorizontalMovementComponent: Velocity component not found"};
    }
}

void KeyboardHorizontalMovementComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = utils::Vector2f{0, 0};
    auto maximumMovementSpeed = velocityComponent->getMaxMovementSpeed();

    if (input.isKeyPressed(input::InputKey::Left))
    {
        currentMovementSpeed.x = -maximumMovementSpeed;
    }

    if (input.isKeyPressed(input::InputKey::Right))
    {
        currentMovementSpeed.x = maximumMovementSpeed;
    }

    velocityComponent->setVelocity(currentMovementSpeed);
}

void KeyboardHorizontalMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input&)
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