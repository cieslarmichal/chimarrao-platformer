#include "ItemHealEffect.h"

#include "health/HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{
ItemHealEffect::ItemHealEffect(unsigned int healthPoints) : healthPoints{healthPoints} {}

void ItemHealEffect::affect(ComponentOwner* target) const
{
    const auto health = target->getComponent<components::core::HealthComponent>();
    if (not health)
    {
        throw exceptions::DependentComponentNotFound{
            "health component not found when trying to affect player with item heal effect"};
    }

    health->gainHealthPoints(healthPoints);
}
}