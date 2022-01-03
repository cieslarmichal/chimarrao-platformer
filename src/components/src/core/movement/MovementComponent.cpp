#include "MovementComponent.h"

namespace components::core
{

MovementComponent::MovementComponent(ComponentOwner* owner, float movementSpeed)
    : Component{owner}, movementSpeed{movementSpeed}
{
}

void MovementComponent::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}

float MovementComponent::getMovementSpeed() const
{
    return movementSpeed;
}

void MovementComponent::allowMoveRight()
{
    canMoveRight = true;
}

void MovementComponent::blockMoveRight()
{
    canMoveRight = false;
}

void MovementComponent::allowMoveLeft()
{
    canMoveLeft = true;
}

void MovementComponent::blockMoveLeft()
{
    canMoveLeft = false;
}

void MovementComponent::allowMoveUp()
{
    canMoveUp = true;
}

void MovementComponent::blockMoveUp()
{
    canMoveUp = false;
}

void MovementComponent::allowMoveDown()
{
    canMoveDown = true;
}

void MovementComponent::blockMoveDown()
{
    canMoveDown = false;
}

bool MovementComponent::isAllowedToMoveRight() const
{
    return canMoveRight;
}

bool MovementComponent::isAllowedToMoveLeft() const
{
    return canMoveLeft;
}

bool MovementComponent::isAllowedToMoveUp() const
{
    return canMoveUp;
}

bool MovementComponent::isAllowedToMoveDown() const
{
    return canMoveDown;
}

void MovementComponent::lock()
{
    locked = true;
}

void MovementComponent::unlock()
{
    locked = false;
}

}