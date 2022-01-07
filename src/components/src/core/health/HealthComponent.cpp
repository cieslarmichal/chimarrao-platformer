#include "HealthComponent.h"

#include "ComponentOwner.h"

namespace components::core
{

HealthComponent::HealthComponent(ComponentOwner* owner, unsigned int initialHealthPoints,
                                 std::function<void(void)> deadAction)
    : Component(owner),
      currentHealthPoints{initialHealthPoints},
      maximumHealthPoints{initialHealthPoints},
      deadAction{std::move(deadAction)}
{
}

void HealthComponent::gainHealthPoints(unsigned int points)
{
    if (dead)
    {
        return;
    }

    if (currentHealthPoints + points < maximumHealthPoints)
    {
        currentHealthPoints += points;
    }
    else
    {
        currentHealthPoints = maximumHealthPoints;
    }
}

void HealthComponent::loseHealthPoints(unsigned int points)
{
    if (dead)
    {
        return;
    }

    if (points <= currentHealthPoints)
    {
        currentHealthPoints -= points;
    }
    else
    {
        currentHealthPoints = 0;
    }

    if (currentHealthPoints == 0)
    {
        dead = true;

        if (deadAction)
        {
            deadAction();
        }
        else
        {
            getOwner().remove();
        }
    }
}

unsigned int HealthComponent::getCurrentHealth() const
{
    return currentHealthPoints;
}

unsigned int HealthComponent::getMaximumHealth() const
{
    return maximumHealthPoints;
}

bool HealthComponent::isDead() const
{
    return dead;
}
}