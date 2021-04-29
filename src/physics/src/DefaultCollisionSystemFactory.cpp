#include "DefaultCollisionSystemFactory.h"

#include "DefaultCollisionSystem.h"

namespace physics
{

std::unique_ptr<CollisionSystem> DefaultCollisionSystemFactory::createCollisionSystem() const
{
    return std::make_unique<DefaultCollisionSystem>();
}
}