#pragma once

#include "Component.h"
#include "UniqueName.h"
#include "ItemEffect.h"

namespace components::core
{
class CollectableItemComponent : public Component
{
public:
    CollectableItemComponent(ComponentOwner* owner, const std::string& name, std::shared_ptr<ItemEffect>);

    void collectBy(ComponentOwner* newCollector);
    void drop();
    void use();
    std::string getName() const;

private:
    ComponentOwner* collector;
    const utils::UniqueName uniqueName;
    std::shared_ptr<ItemEffect> effect;
};
}
