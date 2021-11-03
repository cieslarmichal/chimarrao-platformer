#include "ItemCollectorComponent.h"

#include <cmath>
#include <utility>

#include "core/exceptions/DependentComponentNotFound.h"
#include "core/exceptions/InvalidCapacity.h"

namespace components::core
{
namespace
{
const auto distance = [](const utils::Vector2f& v1, const utils::Vector2f& v2)
{ return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2)); };
}

ItemCollectorComponent::ItemCollectorComponent(ComponentOwner* owner,
                                               std::shared_ptr<physics::Quadtree> quadtree,
                                               std::shared_ptr<physics::RayCast> rayCast, unsigned capacity)
    : Component(owner), collisions{std::move(quadtree)}, rayCast{std::move(rayCast)}, capacity{capacity}
{
    if (capacity == 0)
    {
        throw exceptions::InvalidCapacity{"Capacity can not be 0"};
    }
}

void ItemCollectorComponent::loadDependentComponents()
{
    directionComponent = owner->getComponent<DirectionComponent>();
    if (directionComponent)
    {
        directionComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"ItemCollectorComponent: DirectionComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (boxColliderComponent)
    {
        boxColliderComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ItemCollectorComponent: BoxColliderComponent not found"};
    }
}

void ItemCollectorComponent::collectNearestItem()
{
    if (items.size() >= capacity)
    {
        return;
    }

    const auto closestItem = findNearestItem();
    if (closestItem)
    {
        closestItem->collectBy(owner);
        items.push_back(closestItem);
    }
}

void ItemCollectorComponent::drop(const std::string& itemName)
{
    const auto foundItem = findItemByName(itemName);
    if (foundItem == items.end())
    {
        return;
    }

    const auto placeToDropItem = calculateDropItemPlace();
    if (not placeToDropItem)
    {
        return;
    }

    foundItem->get()->getOwner().transform->setPosition(*placeToDropItem);
    foundItem->get()->drop();
    items.erase(foundItem);
}

void ItemCollectorComponent::use(const std::string& itemName)
{
    const auto foundItem = findItemByName(itemName);
    if (foundItem == items.end())
    {
        return;
    }

    foundItem->get()->use();
    items.erase(foundItem);
}

std::vector<std::shared_ptr<CollectableItemComponent>> ItemCollectorComponent::getItems() const
{
    return items;
}

std::shared_ptr<CollectableItemComponent> ItemCollectorComponent::findNearestItem() const
{
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
        if (not collider->isEnabled())
        {
            continue;
        }

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

    return closestItem;
}

boost::optional<utils::Vector2f> ItemCollectorComponent::calculateDropItemPlace() const
{
    const auto heading = directionComponent->getHeading();
    const auto& ownerPosition = owner->transform->getPosition();
    const auto size = boxColliderComponent->getSize();

    const auto startPoint =
        utils::Vector2f{ownerPosition.x + (size.x / 2.f), ownerPosition.y + (size.y / 2.f)};
    const auto endPoint =
        utils::Vector2f{startPoint.x + (static_cast<float>(heading.x) * dropRange), startPoint.y};

    const auto result = rayCast->cast(startPoint, endPoint, owner->getId(), 2);

    if (result.collision)
    {
        return boost::none;
    }

    return endPoint;
}

std::vector<std::shared_ptr<CollectableItemComponent>>::const_iterator
ItemCollectorComponent::findItemByName(const std::string& itemName) const
{
    return std::find_if(items.begin(), items.end(),
                        [&](const std::shared_ptr<CollectableItemComponent>& item)
                        { return item->getName() == itemName; });
}

}