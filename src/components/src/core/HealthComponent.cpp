#include "HealthComponent.h"

#include "ComponentOwner.h"

namespace components::core
{

HealthComponent::HealthComponent(ComponentOwner* owner, unsigned int initialHealthPoints)
    : Component(owner), currentHealthPoints{initialHealthPoints}, maximumHealthPoints{initialHealthPoints}
{
}

void HealthComponent::gainHealthPoints(unsigned int points)
{
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
        getOwner().remove();
    }

    if (getOwnerName() == "player")
    {
        std::cerr << currentHealthPoints;
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
    return currentHealthPoints == 0;
}
}