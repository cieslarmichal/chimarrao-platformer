#pragma once

#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "PhysicsFactory.h"

namespace physics
{
class DefaultPhysicsFactory : public PhysicsFactory
{
public:
    DefaultPhysicsFactory();

    std::unique_ptr<CollisionSystem> createCollisionSystem() const override;
    std::shared_ptr<DefaultRayCast> createRayCast() const override;
    std::shared_ptr<DefaultQuadtree> getQuadTree() const override;

private:
    std::shared_ptr<DefaultQuadtree> quadtree;
};
}
