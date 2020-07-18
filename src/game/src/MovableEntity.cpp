#include "MovableEntity.h"

namespace game
{
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
}