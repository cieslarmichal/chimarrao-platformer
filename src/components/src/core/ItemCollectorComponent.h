#pragma once

#include "BoxColliderComponent.h"
#include "ComponentOwner.h"
#include "DirectionComponent.h"
#include "RayCast.h"
#include "CollectableItemComponent.h"
#include "Quadtree.h"

namespace components::core
{
class ItemCollectorComponent : public Component
{
public:
    ItemCollectorComponent(ComponentOwner* owner, std::shared_ptr<physics::Quadtree>);

    void collectNearestItem();
    void drop(const std::string& id);
    void use(const std::string& id);
    std::vector<std::shared_ptr<CollectableItemComponent>> getItems() const;

private:
    std::shared_ptr<physics::Quadtree> collisions;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::vector<std::shared_ptr<CollectableItemComponent>> items;
    const unsigned capacity = 8;
};
}
