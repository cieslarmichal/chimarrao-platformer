#pragma once

#include "Entity.h"
#include "Vector.h"

namespace game
{
class NonMovableEntity : public Entity
{
public:
    ~NonMovableEntity() = default;

    double getX() const override;
    double getY() const override;
    double getWidth() const override;
    double getHeight() const override;

private:
    Vector2f position;
    Vector2f size;
};
}