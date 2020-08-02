#include "PhysicsEntity.h"

namespace physics
{

PhysicsEntity::PhysicsEntity(const PhysicsId& initId, const utils::Vector2f& sizeInit,
                             const utils::Vector2f& positionInit)
    : id{initId}, size{sizeInit}, position{positionInit}, movementDirection{0, 0}, movementSpeed{0.0f, 0.0f}
{
}

utils::Vector2f PhysicsEntity::getPosition() const
{
    return position;
}

utils::Vector2f PhysicsEntity::getSize() const
{
    return size;
}

utils::Vector2i PhysicsEntity::getMovementDirection() const
{
    return movementDirection;
}

utils::Vector2f PhysicsEntity::getMovementSpeed() const
{
    return movementSpeed;
}

PhysicsId PhysicsEntity::getPhysicsId() const
{
    return id;
}

void PhysicsEntity::setPosition(const utils::Vector2f& positionToSet)
{
    position = positionToSet;
}

void PhysicsEntity::setMovementDirection(const utils::Vector2i& directionToSet)
{
    movementDirection = directionToSet;
}

void PhysicsEntity::setMovementSpeed(const utils::Vector2f& movementSpeedToSet)
{
    movementSpeed = movementSpeedToSet;
}
}