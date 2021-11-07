#pragma once

#include <memory>

#include "CollisionSystem.h"
#include "DefaultRayCast.h"
#include "PhysicsApi.h"

namespace physics
{
class PHYSICS_API PhysicsFactory
{
public:
    virtual ~PhysicsFactory() = default;

    virtual std::unique_ptr<CollisionSystem> createCollisionSystem() const = 0;
    virtual std::shared_ptr<RayCast> createRayCast() const = 0;
    virtual std::shared_ptr<Quadtree> getQuadTree() const = 0;

    static std::unique_ptr<PhysicsFactory> createCollisionSystemFactory();
};
}
