#include "DefaultCollisionSystem.h"

namespace physics::collisions
{

DefaultCollisionSystem::DefaultCollisionSystem() {}
void DefaultCollisionSystem::add(std::vector<std::shared_ptr<components::core::Component>>&) {}
void DefaultCollisionSystem::processRemovals() {}
void DefaultCollisionSystem::update() {}
void DefaultCollisionSystem::resolve() {}
void DefaultCollisionSystem::processCollisions(
    std::vector<std::shared_ptr<components::core::Component>>& first,
    std::vector<std::shared_ptr<components::core::Component>>& second)
{
}
}