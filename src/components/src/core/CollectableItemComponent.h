#pragma once

#include "Component.h"
#include "ItemEffect.h"
#include "ItemType.h"
#include "UniqueName.h"

namespace components::core
{
class CollectableItemComponent : public Component
{
public:
    CollectableItemComponent(ComponentOwner* owner, const std::string& itemName, ItemType, std::shared_ptr<ItemEffect>);

    void collectBy(ComponentOwner* newCollector);
    void drop();
    void use();
    ItemType getType() const;
    std::string getName() const;

private:
    ComponentOwner* collector;
    const utils::UniqueName uniqueName;
    const ItemType type;
    std::shared_ptr<ItemEffect> effect;
};
}
