#pragma once

#include "PhysicsId.h"
#include "Vector.h"

namespace physics
{
class PhysicsEntity
{
public:
    PhysicsEntity(const PhysicsId&, const utils::Vector2f& size, const utils::Vector2f& position);

    utils::Vector2f getPosition() const;
    utils::Vector2f getSize() const;
    utils::Vector2i getMovementDirection() const;
    utils::Vector2f getMovementSpeed() const;
    PhysicsId getPhysicsId() const;
    void setPosition(const utils::Vector2f& position);
    void setMovementDirection(const utils::Vector2i& direction);
    void setMovementSpeed(const utils::Vector2f& movementSpeed);

private:
    utils::Vector2f position;
    utils::Vector2f size;
    utils::Vector2i movementDirection;
    utils::Vector2f movementSpeed;
    PhysicsId id;
};

inline bool operator==(const PhysicsEntity& lhs, const PhysicsEntity& rhs)
{
    return lhs.getPhysicsId() == rhs.getPhysicsId();
}
}