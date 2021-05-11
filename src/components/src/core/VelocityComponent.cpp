#include "VelocityComponent.h"

namespace components::core
{

VelocityComponent::VelocityComponent(ComponentOwner* owner)
    : Component{owner}, velocity{0.f, 0.f}, maxVelocity{100.f, 100.f}
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
    if (fabs(velocity.x) > maxVelocity.x)
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

    if (fabs(velocity.y) > maxVelocity.y)
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

}