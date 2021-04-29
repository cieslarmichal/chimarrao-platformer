#include "CollisionSystemFactory.h"

#include "DefaultCollisionSystemFactory.h"

namespace physics
{
std::unique_ptr<CollisionSystemFactory> CollisionSystemFactory::createCollisionSystemFactory()
{
    return std::make_unique<DefaultCollisionSystemFactory>();
}
}