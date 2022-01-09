#include "MovementComponent.h"

namespace components::core
{

MovementComponent::MovementComponent(ComponentOwner* owner, float maximumMovementSpeed)
    : Component{owner}, maximumMovementSpeed{maximumMovementSpeed}
{
}

float MovementComponent::getMovementSpeed() const
{
    return maximumMovementSpeed;
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

bool MovementComponent::isLocked() const
{
    return locked;
}

}