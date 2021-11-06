#include "DefaultPhysicsFactory.h"

#include "DefaultCollisionSystem.h"

namespace physics
{

DefaultPhysicsFactory::DefaultPhysicsFactory() : quadtree{std::make_shared<DefaultQuadtree>()} {}

std::unique_ptr<CollisionSystem> DefaultPhysicsFactory::createCollisionSystem() const
{
    return std::make_unique<DefaultCollisionSystem>(quadtree);
}

std::shared_ptr<DefaultRayCast> DefaultPhysicsFactory::createRayCast() const
{
    return std::make_shared<DefaultRayCast>(quadtree);
}

std::shared_ptr<DefaultQuadtree> DefaultPhysicsFactory::getQuadTree() const
{
    return quadtree;
}
}