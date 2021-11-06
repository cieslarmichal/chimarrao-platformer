#pragma once

#include "PhysicsFactory.h"
#include "Quadtree.h"
#include "RayCast.h"

namespace physics
{
class DefaultPhysicsFactory : public PhysicsFactory
{
public:
    DefaultPhysicsFactory();

    std::unique_ptr<CollisionSystem> createCollisionSystem() const override;
    std::shared_ptr<RayCast> createRayCast() const override;
    std::shared_ptr<Quadtree> getQuadTree() const override;

private:
    std::shared_ptr<Quadtree> quadtree;
};
}
