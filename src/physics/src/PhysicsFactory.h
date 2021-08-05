#pragma once

#include <memory>

#include "CollisionSystem.h"
#include "PhysicsApi.h"
#include "RayCast.h"

namespace physics
{
class PHYSICS_API PhysicsFactory
{
public:
    virtual ~PhysicsFactory() = default;

    virtual std::unique_ptr<CollisionSystem> createCollisionSystem() const = 0;
    virtual std::shared_ptr<RayCast> createRayCast() const = 0;

    static std::unique_ptr<PhysicsFactory> createCollisionSystemFactory();
};
}
