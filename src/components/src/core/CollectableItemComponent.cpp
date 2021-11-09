#include "CollectableItemComponent.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"

namespace components::core
{
CollectableItemComponent::CollectableItemComponent(ComponentOwner* owner, const std::string& name, ItemType type,
                                                   std::shared_ptr<ItemEffect> effect)
    : Component(owner), collector{nullptr}, uniqueName{name}, type{type}, effect{std::move(effect)}
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

    collector = nullptr;
    owner->enable();
}

void CollectableItemComponent::use()
{
    if (not collector)
    {
        return;
    }

    effect->affect(collector);
    collector = nullptr;
    owner->remove();
}

ItemType CollectableItemComponent::getType() const
{
    return type;
}

std::string CollectableItemComponent::getName() const
{
    return uniqueName.getName();
}

}