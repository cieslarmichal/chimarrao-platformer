#include "DefaultQuadtree.h"

#include <cmath>

namespace physics
{
namespace
{
double calculateDistanceBetweenRects(const utils::FloatRect& lhs, const utils::FloatRect& rhs)
{
    constexpr auto square = [](const double number) { return number * number; };
    return std::sqrt(square((lhs.top + lhs.height / 2) - (rhs.top + rhs.height / 2)) +
                     square((lhs.left + lhs.width / 2) - (rhs.left + rhs.width / 2)));
}
}

DefaultQuadtree::DefaultQuadtree() : DefaultQuadtree{5, 5, 0, {0, 0, 160, 60}} {}

DefaultQuadtree::DefaultQuadtree(const utils::FloatRect& bounds) : DefaultQuadtree{5, 5, 0, bounds} {}

DefaultQuadtree::DefaultQuadtree(int maxObjectsInNodeBeforeSplitInit, int maxNumberOfSplitsInit,
                                 int treeDepthLevelInit, utils::FloatRect boundsInit)
    : children{nullptr, nullptr, nullptr, nullptr},
      nodeBounds{boundsInit},
      maxObjectsInNodeBeforeSplit{maxObjectsInNodeBeforeSplitInit},
      maxNumberOfSplits{maxNumberOfSplitsInit},
      currentTreeDepthLevel{treeDepthLevelInit}
{
}

void DefaultQuadtree::insertCollider(
    const std::shared_ptr<components::core::BoxColliderComponent>& colliderToInsert)
{
    if (children[0])
    {
        const int indexToPlaceObject =
            getIndexIndicatingToWhichNodeColliderBelongs(colliderToInsert->getCollisionBox());

        if (indexToPlaceObject != thisTreeIndex)
        {
            children[indexToPlaceObject]->insertCollider(colliderToInsert);
        }
        else
        {
            colliders.emplace_back(colliderToInsert);
        }
        return;
    }

    colliders.emplace_back(colliderToInsert);

    if (static_cast<int>(colliders.size()) > maxObjectsInNodeBeforeSplit &&
        currentTreeDepthLevel < maxNumberOfSplits)
    {
        splitIntoChildNodes();
        for (auto& collider : colliders)
        {
            if (const int indexToPlaceObject =
                    getIndexIndicatingToWhichNodeColliderBelongs(collider->getCollisionBox());
                indexToPlaceObject != thisTreeIndex)
            {
                children[indexToPlaceObject]->insertCollider(collider);
            }
        }

        colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
                                       [&](auto& collider) {
                                           return getIndexIndicatingToWhichNodeColliderBelongs(
                                                      collider->getCollisionBox()) != thisTreeIndex;
                                       }),
                        colliders.end());
    }
}

void DefaultQuadtree::removeCollider(
    const std::shared_ptr<components::core::BoxColliderComponent>& colliderToRemove)
{
    const int index = getIndexIndicatingToWhichNodeColliderBelongs(colliderToRemove->getCollisionBox());

    if (index != thisTreeIndex && children[index])
    {
        children[index]->removeCollider(colliderToRemove);
        return;
    }

    colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
                                   [&](auto& collider)
                                   { return collider->getOwnerId() == colliderToRemove->getOwnerId(); }),
                    colliders.end());
}

void DefaultQuadtree::clearAllColliders()
{
    colliders.clear();

    if (children[0])
    {
        for (auto& child : children)
        {
            child->clearAllColliders();
            child = nullptr;
        }
    }
}

std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
DefaultQuadtree::getCollidersIntersectingWithAreaFromX(const utils::FloatRect& area) const
{
    const auto possibleColliders = getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> collidersIntersectingWithArea;

    for (const auto& possibleCollider : possibleColliders)
    {
        auto& possibleColliderBox = possibleCollider->getNextFrameXCollisionBox();

        if (area.intersects(possibleColliderBox))
        {
            if (const auto distance = calculateDistanceBetweenRects(area, possibleColliderBox);
                distance != 0.0)
            {
                collidersIntersectingWithArea.emplace_back(
                    std::shared_ptr<components::core::BoxColliderComponent>(possibleCollider));
            }
        }
    }

    return collidersIntersectingWithArea;
}

std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
DefaultQuadtree::getCollidersIntersectingWithAreaFromY(const utils::FloatRect& area) const
{
    const auto possibleColliders = getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> collidersIntersectingWithArea;

    for (const auto& possibleCollider : possibleColliders)
    {
        auto& possibleColliderBox = possibleCollider->getNextFrameYCollisionBox();

        if (area.intersects(possibleColliderBox))
        {
            if (const auto distance = calculateDistanceBetweenRects(area, possibleColliderBox);
                distance != 0.0)
            {
                collidersIntersectingWithArea.emplace_back(
                    std::shared_ptr<components::core::BoxColliderComponent>(possibleCollider));
            }
        }
    }

    return collidersIntersectingWithArea;
}

const utils::FloatRect& DefaultQuadtree::getNodeBounds() const
{
    return nodeBounds;
}

std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
DefaultQuadtree::getAllCollidersFromQuadtreeNodesIntersectingWithArea(const sf::FloatRect& area) const
{
    auto possibleColliders = colliders;

    if (children[0])
    {
        if (int index = getIndexIndicatingToWhichNodeColliderBelongs(area); index == thisTreeIndex)
        {
            for (const auto& child : children)
            {
                if (child->getNodeBounds().intersects(area))
                {
                    const auto possibleCollidersFromChild =
                        child->getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);
                    possibleColliders.insert(possibleColliders.end(), possibleCollidersFromChild.begin(),
                                             possibleCollidersFromChild.end());
                }
            }
        }
        else
        {
            const auto possibleCollidersFromChild =
                children[index]->getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

            possibleColliders.insert(possibleColliders.end(), possibleCollidersFromChild.begin(),
                                     possibleCollidersFromChild.end());
        }
    }

    possibleColliders.erase(
        std::remove_if(possibleColliders.begin(), possibleColliders.end(),
                       [](const std::shared_ptr<components::core::BoxColliderComponent>& collider)
                       { return !collider->isEnabled(); }),
        possibleColliders.end());

    return possibleColliders;
}

int DefaultQuadtree::getIndexIndicatingToWhichNodeColliderBelongs(const sf::FloatRect& objectBounds) const
{
    const double verticalDividingLine = nodeBounds.left + nodeBounds.width * 0.5f;
    const double horizontalDividingLine = nodeBounds.top + nodeBounds.height * 0.5f;

    const bool north = objectBounds.top < horizontalDividingLine &&
                       (objectBounds.height + objectBounds.top < horizontalDividingLine);
    const bool south = objectBounds.top > horizontalDividingLine;
    const bool west = objectBounds.left < verticalDividingLine &&
                      (objectBounds.left + objectBounds.width < verticalDividingLine);
    const bool east = objectBounds.left > verticalDividingLine;

    if (east)
    {
        if (north)
        {
            return childNorthEastIndex;
        }
        else if (south)
        {
            return childSouthEastIndex;
        }
    }
    else if (west)
    {
        if (north)
        {
            return childNorthWestIndex;
        }
        else if (south)
        {
            return childSouthWestIndex;
        }
    }

    return thisTreeIndex;
}

void DefaultQuadtree::splitIntoChildNodes()
{
    const float childWidth = nodeBounds.width / 2.f;
    const float childHeight = nodeBounds.height / 2.f;

    children[childNorthEastIndex] = std::make_shared<DefaultQuadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left + childWidth, nodeBounds.top, childWidth, childHeight));
    children[childNorthWestIndex] = std::make_shared<DefaultQuadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left, nodeBounds.top, childWidth, childHeight));
    children[childSouthWestIndex] = std::make_shared<DefaultQuadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left, nodeBounds.top + childHeight, childWidth, childHeight));
    children[childSouthEastIndex] = std::make_shared<DefaultQuadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left + childWidth, nodeBounds.top + childHeight, childWidth, childHeight));
}

}