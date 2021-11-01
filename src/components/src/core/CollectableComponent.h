#pragma once

#include "Component.h"

namespace components::core
{
class CollectableComponent : public Component
{
public:
    CollectableComponent(ComponentOwner* owner, ComponentOwner* player);

    void collect();
    void drop();
    void use();

private:
    ComponentOwner* player;
};
}
