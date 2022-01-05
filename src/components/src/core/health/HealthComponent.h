#pragma once

#include "Component.h"

namespace components::core
{
class HealthComponent : public Component
{
public:
    HealthComponent(ComponentOwner* owner, unsigned int initialHealthPoints);

    void gainHealthPoints(unsigned int points);
    void loseHealthPoints(unsigned int points);
    unsigned int getCurrentHealth() const;
    unsigned int getMaximumHealth() const;
    bool isDead() const;

private:
    unsigned int currentHealthPoints;
    unsigned int maximumHealthPoints;
};
}