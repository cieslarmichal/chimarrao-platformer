#include "Quadtree.h"

namespace physics
{
Quadtree::Quadtree() : Quadtree{5, 0, 0, {0, 0, 80, 60}} {}

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
            getIndexIndicatingToWhichNodeColliderBelongs(colliderToInsert->getNextFrameCollisionBox());

        if (indexToPlaceObject != thisTreeIndex)
        {
            children[indexToPlaceObject]->insertCollider(colliderToInsert);
            return;
        }
    }

    colliders.emplace_back(colliderToInsert);

    if (colliders.size() > maxObjectsInNodeBeforeSplit && currentTreeDepthLevel < maxNumberOfSplits)
    {
        splitIntoChildNodes();
        for (auto& collider : colliders)
        {
            if (const int indexToPlaceObject =
                    getIndexIndicatingToWhichNodeColliderBelongs(collider->getNextFrameCollisionBox());
                indexToPlaceObject != thisTreeIndex)
            {
                children[indexToPlaceObject]->insertCollider(collider);
            }
        }

        colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
                                       [&](auto& collider)
                                       {
                                           return getIndexIndicatingToWhichNodeColliderBelongs(
                                                      collider->getNextFrameCollisionBox()) != thisTreeIndex;
                                       }),
                        colliders.end());
    }
}

void Quadtree::removeCollider(const std::shared_ptr<components::core::BoxColliderComponent>& colliderToRemove)
{
    const int index =
        getIndexIndicatingToWhichNodeColliderBelongs(colliderToRemove->getNextFrameCollisionBox());

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
Quadtree::getCollidersIntersectingWithArea(const utils::FloatRect& area) const
{
    const auto possibleColliders = getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);

    std::vector<std::pair<std::shared_ptr<components::core::BoxColliderComponent>, double>>
        collidersIntersectingWithAreaAndDistance;

    for (const auto& possibleCollider : possibleColliders)
    {
        if (area.intersects(possibleCollider->getNextFrameCollisionBox()))
        {
            auto& collider = possibleCollider->getNextFrameCollisionBox();
            double distance = pow((area.top + area.height / 2) - (collider.top + collider.height / 2), 2) +
                              pow((area.left + area.width / 2) - (collider.left + collider.width / 2), 2);
            if (distance > 0.0)
            {
                collidersIntersectingWithAreaAndDistance.emplace_back(
                    std::pair<std::shared_ptr<components::core::BoxColliderComponent>, double>(
                        possibleCollider, distance));
            }
        }
    }
    if(collidersIntersectingWithAreaAndDistance.empty())
    {
        return {};
    }

    double minDistance = std::min_element(collidersIntersectingWithAreaAndDistance.begin(),
                     collidersIntersectingWithAreaAndDistance.end(),
                     [](auto lhs, auto rhs) { return lhs.second < rhs.second; })->second;


    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> collidersIntersectingWithArea;
    for (const auto& pair : collidersIntersectingWithAreaAndDistance)
    {
        if (pair.second / minDistance < 1.3f)
        {
            collidersIntersectingWithArea.push_back(pair.first);
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
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> possibleColliders(colliders.begin(),
                                                                                           colliders.end());

    if (children[0])
    {
        if (int index = getIndexIndicatingToWhichNodeColliderBelongs(area); index == thisTreeIndex)
        {
            for (int i = 0; i < 4; i++)
            {
                if (children[i]->getNodeBounds().intersects(area))
                {
                    const auto possibleCollidersFromChild =
                        children[i]->getAllCollidersFromQuadtreeNodesIntersectingWithArea(area);
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

}