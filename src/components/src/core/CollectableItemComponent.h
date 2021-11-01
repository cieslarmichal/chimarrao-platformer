#pragma once

#include "Component.h"
#include "UniqueName.h"

namespace components::core
{
class CollectableItemComponent : public Component
{
public:
    explicit CollectableItemComponent(ComponentOwner* owner, const std::string& name);

    void collectBy(ComponentOwner* newCollector);
    void drop();
    void use();
    std::string getName() const;

private:
    ComponentOwner* collector;
    const utils::UniqueName uniqueName;
};
}
