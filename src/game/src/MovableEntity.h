#pragma once
#include "Entity.h"
#include "Vector.h"

namespace game
{
class MovableEntity : public Entity
{
public:
    MovableEntity(Vector2f position, Vector2f velocity, Vector2f size);
    ~MovableEntity() = default;

    double getX() const override;
    double getY() const override;
    double getWidth() const override;
    double getHeight() const override;
    Vector2f getVelocity() const;

private:
    Vector2f position;
    Vector2f velocity;
    Vector2f size;
};
}