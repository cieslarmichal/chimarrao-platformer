#include "NonMovableEntity.h"

namespace game
{
NonMovableEntity::NonMovableEntity(Vector2f positionInit, Vector2f sizeInit)
    : position{positionInit}, size{sizeInit}
{
}

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