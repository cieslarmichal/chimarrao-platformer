#pragma once

#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "PhysicsFactory.h"

namespace physics
{
class DefaultPhysicsFactory : public PhysicsFactory
{
public:
    DefaultPhysicsFactory(const utils::FloatRect& mapBoundaries);

    std::unique_ptr<CollisionSystem> createCollisionSystem() const override;
    std::shared_ptr<RayCast> createRayCast() const override;
    std::shared_ptr<Quadtree> getQuadTree() const override;

private:
    std::shared_ptr<DefaultQuadtree> quadtree;
};
}
