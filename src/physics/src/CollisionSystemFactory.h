#pragma once

#include <memory>

#include "CollisionSystem.h"
#include "PhysicsApi.h"

namespace physics
{
class PHYSICS_API CollisionSystemFactory
{
public:
    virtual ~CollisionSystemFactory() = default;

    virtual std::unique_ptr<CollisionSystem> createCollisionSystem() const = 0;

    static std::unique_ptr<CollisionSystemFactory> createCollisionSystemFactory();
};
}
