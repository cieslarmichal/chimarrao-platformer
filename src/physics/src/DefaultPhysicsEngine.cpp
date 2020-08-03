#include "DefaultPhysicsEngine.h"

#include <boost/uuid/uuid_io.hpp>
#include <iostream>

namespace physics
{

static auto& getEntityByPosition(std::vector<PhysicsEntity>& entities,
                                 std::vector<PhysicsEntity>::const_iterator position)
{
    const auto distance = std::vector<PhysicsEntity>::size_type(std::distance(entities.cbegin(), position));
    return entities.at(distance);
}

static auto& getEntityByPosition(const std::vector<PhysicsEntity>& entities,
                                 std::vector<PhysicsEntity>::const_iterator position)
{
    const auto distance = std::vector<PhysicsEntity>::size_type(std::distance(entities.cbegin(), position));
    return entities.at(distance);
}

DefaultPhysicsEngine::DefaultPhysicsEngine() : idGenerator{std::make_unique<PhysicsIdGenerator>()} {}

void DefaultPhysicsEngine::update(const utils::DeltaTime&)
{
    if (not entitiesToRemove.empty())
    {
        cleanUnusedEntities();
    }
}

PhysicsId DefaultPhysicsEngine::acquire(const utils::Vector2f& size, const utils::Vector2f& position)
{
    auto id = PhysicsIdGenerator::generateId();
    entities.emplace_back(id, size, position);
    return id;
}

void DefaultPhysicsEngine::release(const PhysicsId& id)
{
    entitiesToRemove.emplace(id);
}

boost::optional<const utils::Vector2f&> DefaultPhysicsEngine::getPosition(const PhysicsId& id)
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        const auto& entity = getEntityByPosition(entities, entityIter);
        return entity.getPosition();
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
    return boost::none;
}

boost::optional<const utils::Vector2i&> DefaultPhysicsEngine::getMovementDirection(const PhysicsId& id) const
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        const auto& entity = getEntityByPosition(entities, entityIter);
        return entity.getMovementDirection();
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
    return boost::none;
}

boost::optional<const utils::Vector2f&> DefaultPhysicsEngine::getMovementSpeed(const PhysicsId& id) const
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        const auto& entity = getEntityByPosition(entities, entityIter);
        return entity.getMovementSpeed();
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
    return boost::none;
}

void DefaultPhysicsEngine::setPosition(const PhysicsId& id, const utils::Vector2f& positionToSet)
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        auto& entity = getEntityByPosition(entities, entityIter);
        return entity.setPosition(positionToSet);
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
}

void DefaultPhysicsEngine::setMovementDirection(const PhysicsId& id, const utils::Vector2i& directionToSet)
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        auto& entity = getEntityByPosition(entities, entityIter);
        return entity.setMovementDirection(directionToSet);
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
}

void DefaultPhysicsEngine::setMovementSpeed(const PhysicsId& id, const utils::Vector2f& movementSpeedToSet)
{
    const auto entityIter = findEntityPosition(id);
    if (entityIter != entities.end())
    {
        auto& entity = getEntityByPosition(entities, entityIter);
        return entity.setMovementSpeed(movementSpeedToSet);
    }
    std::cerr << "Physics entity with id: " << id << " not found\n";
}

std::vector<PhysicsEntity>::const_iterator
DefaultPhysicsEngine::findEntityPosition(const PhysicsId& physicsIdToFind) const
{
    return std::find_if(entities.begin(), entities.end(), [&physicsIdToFind](const PhysicsEntity& entity) {
        return entity.getPhysicsId() == physicsIdToFind;
    });
}

void DefaultPhysicsEngine::cleanUnusedEntities()
{
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [&](const PhysicsEntity& entity) {
                                      return entitiesToRemove.count(entity.getPhysicsId());
                                  }),
                   entities.end());
    entitiesToRemove.clear();
}

}