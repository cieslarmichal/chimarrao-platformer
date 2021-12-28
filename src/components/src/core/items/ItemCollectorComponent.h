#pragma once

#include "BoxColliderComponent.h"
#include "CollectableItemComponent.h"
#include "ComponentOwner.h"
#include "DirectionComponent.h"
#include "ItemInfo.h"
#include "Quadtree.h"
#include "RayCast.h"
#include "Timer.h"

namespace components::core
{
class ItemCollectorComponent : public Component
{
public:
    ItemCollectorComponent(ComponentOwner* owner, std::shared_ptr<physics::Quadtree>,
                           std::shared_ptr<physics::RayCast>, unsigned capacity,
                           std::shared_ptr<utils::Timer>);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void collectNearestItem();
    void drop(const std::string& itemName);
    void use(const std::string& itemName);
    std::vector<ItemInfo> getItemsInfo() const;

private:
    std::shared_ptr<CollectableItemComponent> findNearestItem() const;
    boost::optional<utils::Vector2f> calculateDropItemPlace() const;
    std::vector<std::shared_ptr<CollectableItemComponent>>::const_iterator
    findItemByName(const std::string& itemName) const;

    std::shared_ptr<physics::Quadtree> collisions;
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::vector<std::shared_ptr<CollectableItemComponent>> items;
    const unsigned capacity;
    const float dropRange{4};
    const float timeAfterNextItemCanBeCollected;
    std::shared_ptr<utils::Timer> possibilityToCollectNextItemTimer;
};
}
