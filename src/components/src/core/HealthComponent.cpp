#include "HealthComponent.h"

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
}

unsigned int HealthComponent::getHealth() const
{
    return currentHealthPoints;
}

bool HealthComponent::isDead() const
{
    return currentHealthPoints == 0;
}
}