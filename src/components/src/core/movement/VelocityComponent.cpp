#include "VelocityComponent.h"

#include <cmath>

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

VelocityComponent::VelocityComponent(ComponentOwner* owner, float maxMovementSpeed,
                                     const utils::Vector2f& velocityInit)
    : Component{owner}, velocity{velocityInit}, maxVelocity{100.f, 100.f}, maxMovementSpeed{maxMovementSpeed}
{
}

void VelocityComponent::setVelocity(const sf::Vector2f& vel)
{
    velocity = vel;
    trimVelocity();
}

void VelocityComponent::setVelocity(float x, float y)
{
    velocity = {x, y};
    trimVelocity();
}

const sf::Vector2f& VelocityComponent::getVelocity() const
{
    return velocity;
}

void VelocityComponent::trimVelocity()
{
    if (std::fabs(velocity.x) > maxVelocity.x)
    {
        if (velocity.x > 0.f)
        {
            velocity.x = maxVelocity.x;
        }
        else
        {
            velocity.x = -maxVelocity.x;
        }
    }

    if (std::fabs(velocity.y) > maxVelocity.y)
    {
        if (velocity.y > 0.f)
        {
            velocity.y = maxVelocity.y;
        }
        else
        {
            velocity.y = -maxVelocity.y;
        }
    }
}

float VelocityComponent::getMaxMovementSpeed() const
{
    return maxMovementSpeed;
}
}