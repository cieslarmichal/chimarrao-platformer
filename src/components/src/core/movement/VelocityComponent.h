#pragma once

#include "ComponentOwner.h"

namespace components::core
{
class VelocityComponent : public Component
{
public:
    explicit VelocityComponent(ComponentOwner* owner);

    void setVelocity(const sf::Vector2f& vel);
    void setVelocity(float x, float y);
    const sf::Vector2f& getVelocity() const;

private:
    void trimVelocity();

    sf::Vector2f velocity;
    sf::Vector2f maxVelocity;
};
}
