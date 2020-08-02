#pragma once

#include "PhysicsEngine.h"
#include "PhysicsIdGenerator.h"
#include "PhysicsEntity.h"
#include <memory>
#include <vector>
#include <boost/functional/hash.hpp>
#include <unordered_set>

namespace physics
{
class DefaultPhysicsEngine : public PhysicsEngine
{
public:
    DefaultPhysicsEngine();

    void update(const utils::DeltaTime&) override;
    PhysicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position) override;
    void release(const PhysicsId&) override;
    utils::Vector2f getPosition(const PhysicsId&) override;
    utils::Vector2i getMovementDirection(const PhysicsId&) const override;
    utils::Vector2f getMovementSpeed(const PhysicsId&) const override;
    void setPosition(const PhysicsId&, const utils::Vector2f&) override;
    void setMovementDirection(const PhysicsId&, const utils::Vector2i& direction) override;
    void setMovementSpeed(const PhysicsId&, const utils::Vector2f& movementSpeed) override;

private:
    std::vector<PhysicsEntity>::const_iterator
    findEntityPosition(const PhysicsId& physicsIdToFind) const;
    void cleanUnusedEntities();

    std::unique_ptr<PhysicsIdGenerator> idGenerator;
    std::vector<PhysicsEntity> entities;
    std::unordered_set<PhysicsId, boost::hash<PhysicsId>> entitiesToRemove;
};
}