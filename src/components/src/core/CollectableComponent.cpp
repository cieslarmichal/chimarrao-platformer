#include "CollectableComponent.h"
#include "ComponentOwner.h"

namespace components::core
{

CollectableComponent::CollectableComponent(ComponentOwner* owner, ComponentOwner* player)
    : Component(owner), player{player}
{
}

void CollectableComponent::collect()
{
//    owner->disable();
}

void CollectableComponent::drop() {}
void CollectableComponent::use() {}
}