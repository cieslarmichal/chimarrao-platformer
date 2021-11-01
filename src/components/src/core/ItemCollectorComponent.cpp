#include "ItemCollectorComponent.h"

#include <cmath>
#include <utility>

#include "core/exceptions/DependentComponentNotFound.h"

namespace components::core
{
namespace
{
const auto distance = [](const utils::Vector2f& v1, const utils::Vector2f& v2)
{ return std::pow(std::sqrt(v2.x - v1.x) + std::sqrt(v2.y - v1.y), 2); };
}

ItemCollectorComponent::ItemCollectorComponent(ComponentOwner* owner,
                                               std::shared_ptr<physics::Quadtree> quadtree,
                                               std::shared_ptr<physics::RayCast> rayCast)
    : Component(owner), collisions{std::move(quadtree)}, rayCast{std::move(rayCast)}
{
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
        throw exceptions::DependentComponentNotFound{"AttackComponent: DirectionComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (boxColliderComponent)
    {
        boxColliderComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"AttackComponent: BoxColliderComponent not found"};
    }
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

void ItemCollectorComponent::drop(const std::string& itemName)
{
    const auto foundItem = std::find_if(items.begin(), items.end(),
                                        [&](const std::shared_ptr<CollectableItemComponent>& item)
                                        { return item->getName() == itemName; });
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
    const auto foundItem = std::find_if(items.begin(), items.end(),
                                        [&](const std::shared_ptr<CollectableItemComponent>& item)
                                        { return item->getName() == itemName; });
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

}