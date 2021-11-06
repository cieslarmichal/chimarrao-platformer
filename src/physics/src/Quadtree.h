#pragma once

#include <array>

#include "core/BoxColliderComponent.h"
#include "core/ComponentOwner.h"

namespace physics
{
class Quadtree
{
public:
    virtual ~Quadtree() = default;

    virtual void insertCollider(const std::shared_ptr<components::core::BoxColliderComponent>&) = 0;
    virtual void removeCollider(const std::shared_ptr<components::core::BoxColliderComponent>&) = 0;
    virtual void clearAllColliders() = 0;
    virtual const utils::FloatRect& getNodeBounds() const = 0;
    virtual std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getCollidersIntersectingWithAreaFromX(const utils::FloatRect& area) const = 0;
    virtual std::vector<std::shared_ptr<components::core::BoxColliderComponent>>
    getCollidersIntersectingWithAreaFromY(const utils::FloatRect& area) const = 0;
};
}