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
    // TODO: enum
    // Returns the index for the node that will contain
    // the object. -1 is returned if it is this node.
    int getIndexIndicatingToWhichNodeColliderBelongs(const sf::FloatRect& objectBounds);
    void splitIntoChildNodes();

    static const int thisTree = -1;
    static const int childNE = 0;
    static const int childNW = 1;
    static const int childSW = 2;
    static const int childSE = 3;

    Quadtree* parent;
    std::array<std::shared_ptr<Quadtree>, 4> children;
    std::vector<std::shared_ptr<components::core::BoxColliderComponent>> colliders;
    utils::FloatRect nodeBounds;
    int maxObjectsInNodeBeforeSplit;
    int maxNumberOfSplits;
    int currentTreeDepthLevel;
};
}