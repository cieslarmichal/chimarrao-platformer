#include "KeyboardHorizontalMovementComponent.h"

namespace components::core
{

KeyboardHorizontalMovementComponent::KeyboardHorizontalMovementComponent(ComponentOwner* ownerInit)
    : MovementComponent{ownerInit, 16.f}
{
}

void KeyboardHorizontalMovementComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = utils::Vector2f{0, 0};

    if (input.isKeyPressed(input::InputKey::Left))
    {
        currentMovementSpeed.x = -movementSpeed;
    }

    if (input.isKeyPressed(input::InputKey::Right))
    {
        currentMovementSpeed.x = movementSpeed;
    }

    const float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    const float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}
}