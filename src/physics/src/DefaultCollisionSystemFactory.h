#pragma once

#include "CollisionSystemFactory.h"

namespace physics
{
class DefaultCollisionSystemFactory : public CollisionSystemFactory
{
public:
    std::unique_ptr<CollisionSystem> createCollisionSystem() const override;
};
}
