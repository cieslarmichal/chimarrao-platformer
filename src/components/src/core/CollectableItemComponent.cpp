#include "CollectableItemComponent.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"

namespace components::core
{
CollectableItemComponent::CollectableItemComponent(ComponentOwner* owner, const std::string& name)
    : Component(owner), collector{nullptr}, uniqueName{name}
{
}

void CollectableItemComponent::collectBy(ComponentOwner* newCollector)
{
    collector = newCollector;

    owner->disable();
}

void CollectableItemComponent::drop()
{
    if (not collector)
    {
        return;
    }

    owner->enable();
}

void CollectableItemComponent::use()
{
    if (not collector)
    {
        return;
    }

    collector->getComponent<components::core::HealthComponent>()->gainHealthPoints(1);

    owner->remove();
}

std::string CollectableItemComponent::getName() const
{
    return uniqueName.getName();
}
}