#include "ItemCollectorComponent.h"

#include <cmath>
#include <utility>

namespace components::core
{
namespace
{
const auto distance = [](const utils::Vector2f& v1, const utils::Vector2f& v2)
{ return std::pow(std::sqrt(v2.x - v1.x) + std::sqrt(v2.y - v1.y), 2); };
}

ItemCollectorComponent::ItemCollectorComponent(ComponentOwner* owner,
                                               std::shared_ptr<physics::Quadtree> quadtree)
    : Component(owner), collisions{std::move(quadtree)}
{
}

void ItemCollectorComponent::collectNearestItem()
{
    if (items.size() >= capacity)
    {
        return;
    }

    const auto& ownerPosition = owner->transform->getPosition();
    const auto ownerSize = boxColliderComponent->getSize();

    const auto startPoint = utils::Vector2f{ownerPosition.x + (ownerSize.x / 2.f) - 3.f,
                                            ownerPosition.y + (ownerSize.y / 2.f) - 3.f};
    const auto searchSize = utils::Vector2f{6.f, 6.f};
    const auto collisionArea = utils::FloatRect{startPoint, searchSize};
    const auto colliders = collisions->getCollidersIntersectingWithAreaFromX(collisionArea);

    std::shared_ptr<components::core::CollectableItemComponent> closestItem;
    double closestDistance = 1000.0;

    for (const auto& collider : colliders)
    {
        const auto itemCollider = collider->getOwner().getComponent<CollectableItemComponent>();
        if (not itemCollider)
        {
            continue;
        }

        const auto distanceToItem = distance(collider->getOwner().transform->getPosition(), ownerPosition);
        if (distanceToItem < closestDistance)
        {
            closestItem = itemCollider;
            closestDistance = distanceToItem;
        }
    }


    closestItem->collectBy(owner);
    items.push_back(closestItem);
}

void ItemCollectorComponent::drop(const std::string& id)
{

}

void ItemCollectorComponent::use(const std::string& id) {}

std::vector<std::shared_ptr<CollectableItemComponent>> ItemCollectorComponent::getItems() const
{
    return items;
}
}