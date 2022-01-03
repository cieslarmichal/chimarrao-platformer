#include "PhysicsFactory.h"

#include "DefaultPhysicsFactory.h"

namespace physics
{
std::unique_ptr<PhysicsFactory> PhysicsFactory::createPhysicsFactory(const utils::FloatRect& mapBounds)
{
    return std::make_unique<DefaultPhysicsFactory>(mapBounds);
}
}