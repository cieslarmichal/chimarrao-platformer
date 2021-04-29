#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Bitmask.h"
#include "CollisionSystem.h"
#include "Quadtree.h"
#include "core/BoxColliderComponent.h"
#include "core/CollisionLayer.h"

namespace physics
{
class DefaultCollisionSystem : public CollisionSystem
{
public:
    DefaultCollisionSystem();

    void add(std::vector<std::shared_ptr<components::core::ComponentOwner>>&) override;
    void processRemovals() override;
    void update() override;

private:
    void resolve();

    std::map<components::core::CollisionLayer, utils::Bitmask> possibleCollisionsInLayers;
    std::map<components::core::CollisionLayer,
             std::vector<std::shared_ptr<components::core::BoxColliderComponent>>>
        collidersPerLayers;
    Quadtree collisionTree;
};
}
