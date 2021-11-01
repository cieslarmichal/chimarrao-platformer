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
    ItemCollectorComponent(ComponentOwner* owner, std::shared_ptr<physics::Quadtree>, std::shared_ptr<physics::RayCast>);

    void loadDependentComponents() override;
    void collectNearestItem();
    void drop(const std::string& itemName);
    void use(const std::string& itemName);
    std::vector<std::shared_ptr<CollectableItemComponent>> getItems() const;

private:
    boost::optional<utils::Vector2f> calculateDropItemPlace() const;

    std::shared_ptr<physics::Quadtree> collisions;
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::vector<std::shared_ptr<CollectableItemComponent>> items;
    const unsigned capacity = 8;
    const float dropRange{4};
};
}
