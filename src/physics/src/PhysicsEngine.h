#pragma once

#include <boost/optional.hpp>

#include "DeltaTime.h"
#include "PhysicsId.h"
#include "Vector.h"

namespace physics
{
class PhysicsEngine
{
public:
    virtual ~PhysicsEngine() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual PhysicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position) = 0;
    virtual void release(const PhysicsId&) = 0;
    virtual boost::optional<const utils::Vector2f&> getPosition(const PhysicsId&) = 0;
    virtual boost::optional<const utils::Vector2i&> getMovementDirection(const PhysicsId&) const = 0;
    virtual boost::optional<const utils::Vector2f&> getMovementSpeed(const PhysicsId&) const = 0;
    virtual void setPosition(const PhysicsId&, const utils::Vector2f& position) = 0;
    virtual void setMovementDirection(const PhysicsId&, const utils::Vector2i& direction) = 0;
    virtual void setMovementSpeed(const PhysicsId&, const utils::Vector2f& movementSpeed) = 0;
};
}