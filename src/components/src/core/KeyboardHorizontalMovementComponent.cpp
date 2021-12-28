#include "KeyboardHorizontalMovementComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardHorizontalMovementComponent::KeyboardHorizontalMovementComponent(ComponentOwner* ownerInit)
    : MovementComponent{ownerInit, 6.f}
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
            "KeyboardMovementComponent: Velocity component not found"};
    }
}

void KeyboardHorizontalMovementComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (input.isKeyPressed(input::InputKey::Left))
    {
        currentMovementSpeed.x = -movementSpeed;
    }

    if (input.isKeyPressed(input::InputKey::Right))
    {
        currentMovementSpeed.x = movementSpeed;
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

    velocityComponent->setVelocity(currentMovementSpeed);
    const float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    const float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}
}