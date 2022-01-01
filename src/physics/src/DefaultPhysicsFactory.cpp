#include "DefaultPhysicsFactory.h"

#include "DefaultCollisionSystem.h"

namespace physics
{

DefaultPhysicsFactory::DefaultPhysicsFactory(const utils::FloatRect& mapBoundaries)
    : quadtree{std::make_shared<DefaultQuadtree>(mapBoundaries)}
{
}

std::unique_ptr<CollisionSystem> DefaultPhysicsFactory::createCollisionSystem() const
{
    return std::make_unique<DefaultCollisionSystem>(quadtree);
}

std::shared_ptr<RayCast> DefaultPhysicsFactory::createRayCast() const
{
    return std::make_shared<DefaultRayCast>(quadtree);
}

std::shared_ptr<Quadtree> DefaultPhysicsFactory::getQuadTree() const
{
    return quadtree;
}
}