#pragma once

#include <boost/functional/hash.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

#include "PhysicsEngine.h"
#include "PhysicsEntity.h"
#include "PhysicsIdGenerator.h"

namespace physics
{
class DefaultPhysicsEngine : public PhysicsEngine
{
public:
    DefaultPhysicsEngine();

    void update(const utils::DeltaTime&) override;
    PhysicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position) override;
    void release(const PhysicsId&) override;
    boost::optional<const utils::Vector2f&> getPosition(const PhysicsId&) override;
    boost::optional<const utils::Vector2i&> getMovementDirection(const PhysicsId&) const override;
    boost::optional<const utils::Vector2f&> getMovementSpeed(const PhysicsId&) const override;
    void setPosition(const PhysicsId&, const utils::Vector2f& position) override;
    void setMovementDirection(const PhysicsId&, const utils::Vector2i& direction) override;
    void setMovementSpeed(const PhysicsId&, const utils::Vector2f& movementSpeed) override;

private:
    std::vector<PhysicsEntity>::const_iterator findEntityPosition(const PhysicsId&) const;
    void cleanUnusedEntities();

    std::unique_ptr<PhysicsIdGenerator> idGenerator;
    std::vector<PhysicsEntity> entities;
    std::unordered_set<PhysicsId, boost::hash<PhysicsId>> entitiesToRemove;
};
}