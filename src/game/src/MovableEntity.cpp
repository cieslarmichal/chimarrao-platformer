#include "MovableEntity.h"

namespace game
{
MovableEntity::MovableEntity(Vector2f positionInit, Vector2f velocityInit, Vector2f sizeInit)
    : position{positionInit}, velocity{velocityInit}, size{sizeInit}
{
}

double MovableEntity::getX() const
{
    return position.x;
}

double MovableEntity::getY() const
{
    return position.y;
}

double MovableEntity::getWidth() const
{
    return size.x;
}

double MovableEntity::getHeight() const
{
    return size.y;
}

Vector2f MovableEntity::getVelocity() const
{
    return velocity;
}
}