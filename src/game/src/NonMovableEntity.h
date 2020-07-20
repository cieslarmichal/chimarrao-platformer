#pragma once

#include "Entity.h"
#include "Vector.h"

namespace game
{
class NonMovableEntity : public Entity
{
public:
    NonMovableEntity(utils::Vector2f positionInit, utils::Vector2f sizeInit);
    ~NonMovableEntity() = default;

    double getX() const override;
    double getY() const override;
    double getWidth() const override;
    double getHeight() const override;

private:
    utils::Vector2f position;
    utils::Vector2f size;
};
}