#pragma once

#include <map>
#include <memory>
#include <vector>

#include "CollisionSystem.h"
#include "Quadtree.h"
#include "core/BoxColliderComponent.h"
#include "core/CollisionLayer.h"
#include "Bitmask.h"

namespace physics::collisions
{
class DefaultCollisionSystem : CollisionSystem
{
public:
    DefaultCollisionSystem();

    void add(std::vector<std::shared_ptr<components::core::Component>>&) override;
    void processRemovals() override;
    void update() override;

private:
    void resolve();
    void processCollisions(std::vector<std::shared_ptr<components::core::Component>>& first,
                           std::vector<std::shared_ptr<components::core::Component>>& second);

    std::map<components::core::CollisionLayer, utils::Bitmask> collisionLayers;

    std::map<components::core::CollisionLayer,
             std::vector<std::shared_ptr<components::core::BoxColliderComponent>>>
        colliders;

    Quadtree collisionTree;
};
}
