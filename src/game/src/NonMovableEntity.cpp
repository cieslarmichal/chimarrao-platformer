#include "NonMovableEntity.h"

namespace game
{
double NonMovableEntity::getX() const
{
    return position.x;
}

double NonMovableEntity::getY() const
{
    return position.y;
}

double NonMovableEntity::getWidth() const
{
    return size.x;
}

double NonMovableEntity::getHeight() const
{
    return size.y;
}
}