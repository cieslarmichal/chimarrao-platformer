#pragma once

namespace game
{
class Entity
{
public:
    virtual ~Entity() = default;

    virtual double getX() const = 0;
    virtual double getY() const = 0;
    virtual double getWidth() const = 0;
    virtual double getHeight() const = 0;
};
}