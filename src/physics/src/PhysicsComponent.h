#pragma once

#include "Vector.h"
#include "DeltaTime.h"
#include "Rectangle.h"

namespace physics
{
class PhysicsComponent
{
public:
    virtual ~PhysicsComponent() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void move(const utils::DeltaTime&) = 0;
    virtual void setPosition(const utils::Vector2f&) = 0;
    virtual utils::Vector2f getPosition() const = 0;
    virtual utils::Vector2f getSize() const = 0;
    virtual float getLeftEdge() const = 0;
    virtual float getRightEdge() const = 0;
    virtual float getBottomEdge() const = 0;
    virtual float getUpperEdge() const = 0;
    virtual void setDirection(const utils::Vector2i&) = 0;
    virtual const Rectangle& getRectangle() const = 0;
};
}