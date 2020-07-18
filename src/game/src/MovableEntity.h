#pragma once
#include "Vector.h"
#include "Entity.h"

namespace game
{
class MovableEntity : public Entity
{
public:
    MovableEntity(utils::Vector2f position, utils::Vector2f velocity, utils::Vector2f size);
    ~MovableEntity() = default;

    double getX() const override;
    double getY() const override;
    double getWidth() const override;
    double getHeight() const override;
    utils::Vector2f getVelocity() const;

private:
    utils::Vector2f position;
    utils::Vector2f velocity;
    utils::Vector2f size;
};
}