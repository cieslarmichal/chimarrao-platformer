#pragma once

#include "PhysicsComponent.h"
#include "Rectangle.h"

namespace physics
{
class PhysicsSfmlComponent : public PhysicsComponent
{
public:
    PhysicsSfmlComponent(const utils::Vector2f& size, const utils::Vector2f& position);

    void update(const utils::DeltaTime&) override;
    void move(const utils::DeltaTime&) override;
    void setPosition(const utils::Vector2f&) override;
    utils::Vector2f getPosition() const override;
    utils::Vector2f getSize() const override;
    float getLeftEdge() const override;
    float getRightEdge() const override;
    float getBottomEdge() const override;
    float getUpperEdge() const override;
    void setDirection(const utils::Vector2i&) override;
    const Rectangle& getRectangle() const override;

private:
    Rectangle rectangle;
    utils::Vector2i direction;
    utils::Vector2f movementSpeed;
};
}