#include "PhysicsFactory.h"

#include "DefaultPhysicsFactory.h"

namespace physics
{
std::unique_ptr<PhysicsFactory> PhysicsFactory::createCollisionSystemFactory()
{
    return std::make_unique<DefaultPhysicsFactory>();
}
}