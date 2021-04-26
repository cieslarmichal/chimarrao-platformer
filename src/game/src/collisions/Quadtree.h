#pragma once

#include <array>

#include "core/BoxColliderComponent.h"
#include "core/ComponentOwner.h"

namespace game::collisions
{
class Quadtree
{
public:
    Quadtree();
    Quadtree(Quadtree* parent, int maxObjectsInNodeBeforeSplit, int maxNumberOfSplits, int treeDepthLevel,
             utils::FloatRect bounds);

    void insertCollider(const std::shared_ptr<components::core::BoxColliderComponent>&);
    void removeCollider(const std::shared_ptr<components::core::BoxColliderComponent>&);
    void clearAllColliders();
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getCollidersIntersectingWithArea(const utils::FloatRect& area);
    const utils::FloatRect& getNodeBounds();

private:
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getAllCollidersFromQuadtreeNodesIntersectingWithArea(const sf::FloatRect& area);
    int getIndexIndicatingToWhichNodeColliderBelongs(const sf::FloatRect& objectBounds);
    void splitIntoChildNodes();

    static const int thisTreeIndex = -1;
    static const int childNorthEastIndex = 0;
    static const int childNorthWestIndex = 1;
    static const int childSouthWestIndex = 2;
    static const int childSouthEastIndex = 3;

    Quadtree* parent;
    std::array<std::shared_ptr<Quadtree>, 4> children;
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> colliders;
    utils::FloatRect nodeBounds;
    const int maxObjectsInNodeBeforeSplit;
    const int maxNumberOfSplits;
    int currentTreeDepthLevel;
};
}