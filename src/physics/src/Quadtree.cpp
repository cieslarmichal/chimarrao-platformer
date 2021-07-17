#include "Quadtree.h"

#include <cmath>

namespace physics
{
Quadtree::Quadtree() : Quadtree{5, 5, 0, {0, 0, 160, 60}} {}

Quadtree::Quadtree(int maxObjectsInNodeBeforeSplitInit, int maxNumberOfSplitsInit, int treeDepthLevelInit,
                   utils::FloatRect boundsInit)
    : maxObjectsInNodeBeforeSplit{maxObjectsInNodeBeforeSplitInit},
      maxNumberOfSplits{maxNumberOfSplitsInit},
      currentTreeDepthLevel{treeDepthLevelInit},
      nodeBounds{boundsInit},
      children{nullptr, nullptr, nullptr, nullptr}
{
}

void Quadtree::insertCollider(const std::shared_ptr<components::core::BoxColliderComponent>& colliderToInsert)
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

void Quadtree::removeCollider(const std::shared_ptr<components::core::BoxColliderComponent>& colliderToRemove)
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

void Quadtree::clearAllColliders()
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
Quadtree::getCollidersIntersectingWithAreaFromX(const utils::FloatRect& area) const
{
    const auto possibleColliders = getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> collidersIntersectingWithArea;

    for (const auto& possibleCollider : possibleColliders)
    {
        auto& possibleColliderBox = possibleCollider->getNextFrameXCollisionBox();

        if (area.intersects(possibleColliderBox))
        {
            if (const auto distance = calculateDistanceBetweenRect(area, possibleColliderBox);
                distance != 0.0)
            {
                collidersIntersectingWithArea.emplace_back(
                    std::shared_ptr<components::core::BoxColliderComponent>(
                        possibleCollider));
            }
        }
    }

    return collidersIntersectingWithArea;
}

std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
Quadtree::getCollidersIntersectingWithAreaFromY(const utils::FloatRect& area) const
{
    const auto possibleColliders = getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> collidersIntersectingWithArea;

    for (const auto& possibleCollider : possibleColliders)
    {
        auto& possibleColliderBox = possibleCollider->getNextFrameYCollisionBox();

        if (area.intersects(possibleColliderBox))
        {
            if (const auto distance = calculateDistanceBetweenRect(area, possibleColliderBox);
                distance != 0.0)
            {
                collidersIntersectingWithArea.emplace_back(
                    std::shared_ptr<components::core::BoxColliderComponent>(
                        possibleCollider));
            }
        }
    }

    return collidersIntersectingWithArea;
}

const utils::FloatRect& Quadtree::getNodeBounds() const
{
    return nodeBounds;
}

std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
Quadtree::getAllCollidersFromQuadtreeNodesIntersectingWithArea(const sf::FloatRect& area) const
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

    return possibleColliders;
}

int Quadtree::getIndexIndicatingToWhichNodeColliderBelongs(const sf::FloatRect& objectBounds) const
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

void Quadtree::splitIntoChildNodes()
{
    const float childWidth = nodeBounds.width / 2.f;
    const float childHeight = nodeBounds.height / 2.f;

    children[childNorthEastIndex] = std::make_shared<Quadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left + childWidth, nodeBounds.top, childWidth, childHeight));
    children[childNorthWestIndex] =
        std::make_shared<Quadtree>(maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
                                   sf::FloatRect(nodeBounds.left, nodeBounds.top, childWidth, childHeight));
    children[childSouthWestIndex] = std::make_shared<Quadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left, nodeBounds.top + childHeight, childWidth, childHeight));
    children[childSouthEastIndex] = std::make_shared<Quadtree>(
        maxObjectsInNodeBeforeSplit, maxNumberOfSplits, currentTreeDepthLevel + 1,
        sf::FloatRect(nodeBounds.left + childWidth, nodeBounds.top + childHeight, childWidth, childHeight));
}

double Quadtree::calculateDistanceBetweenRect(const utils::FloatRect& lhs, const utils::FloatRect& rhs)
{
    constexpr auto square = [](const double number) { return number * number; };
    return square((lhs.top + lhs.height / 2) - (rhs.top + rhs.height / 2)) *
           square((lhs.left + lhs.width / 2) - (rhs.left + rhs.width / 2));
}

}