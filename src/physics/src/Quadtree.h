#pragma once

#include <array>

#include "core/BoxColliderComponent.h"
#include "core/ComponentOwner.h"

namespace physics
{
class Quadtree
{
public:
    Quadtree();
    Quadtree(int maxObjectsInNodeBeforeSplit, int maxNumberOfSplits, int treeDepthLevel,
             utils::FloatRect bounds);

    void insertCollider(const std::shared_ptr<components::core::BoxColliderComponent>&);
    void removeCollider(const std::shared_ptr<components::core::BoxColliderComponent>&);
    void clearAllColliders();
    const utils::FloatRect& getNodeBounds() const;
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getCollidersIntersectingWithAreaFromX(const utils::FloatRect& area) const;
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getCollidersIntersectingWithAreaFromY(const utils::FloatRect& area) const;

private:
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getAllCollidersFromQuadtreeNodesIntersectingWithArea(const sf::FloatRect& area) const;
    int getIndexIndicatingToWhichNodeColliderBelongs(const sf::FloatRect& objectBounds) const;
    void splitIntoChildNodes();

    std::array<std::shared_ptr<Quadtree>, 4> children;
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> colliders;
    utils::FloatRect nodeBounds;
    const int maxObjectsInNodeBeforeSplit;
    const int maxNumberOfSplits;
    int currentTreeDepthLevel;

    static const int thisTreeIndex = -1;
    static const int childNorthEastIndex = 0;
    static const int childNorthWestIndex = 1;
    static const int childSouthWestIndex = 2;
    static const int childSouthEastIndex = 3;
};
}