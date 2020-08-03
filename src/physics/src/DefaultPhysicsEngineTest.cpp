#include "DefaultPhysicsEngine.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace physics;

namespace
{
const utils::Vector2f size1{20, 30};
const utils::Vector2f size2{100, 100};
const utils::Vector2f position1{0, 10};
const utils::Vector2f position2{42, 102};
const utils::DeltaTime deltaTime{0.0001};
const utils::Vector2f initialMovementSpeed{0, 0};
const utils::Vector2f movementSpeed{5, 5};
const utils::Vector2i initialMovementDirection{0, 0};
const utils::Vector2i movementDirection{1, 0};
}

class DefaultPhysicsEngineTest : public Test
{
public:
    DefaultPhysicsEngine physicsEngine;
};

TEST_F(DefaultPhysicsEngineTest, aquiredEntity_shouldExistsInEngine)
{
    const auto entityId = physicsEngine.acquire(size1, position1);

    ASSERT_TRUE(physicsEngine.getPosition(entityId));
}

TEST_F(DefaultPhysicsEngineTest, releasedEntity_afterUpdateShouldNotExistInEngine)
{
    const auto entityId = physicsEngine.acquire(size1, position1);

    physicsEngine.release(entityId);
    physicsEngine.update(deltaTime);

    ASSERT_FALSE(physicsEngine.getPosition(entityId));
}

TEST_F(DefaultPhysicsEngineTest, releasedOneOfTwoEntities_afterUpdateShouldRemoveOneEntity)
{
    const auto entityId1 = physicsEngine.acquire(size1, position1);
    const auto entityId2 = physicsEngine.acquire(size2, position2);

    physicsEngine.release(entityId1);
    physicsEngine.update(deltaTime);

    ASSERT_FALSE(physicsEngine.getPosition(entityId1));
    ASSERT_TRUE(physicsEngine.getPosition(entityId2));
}

TEST_F(DefaultPhysicsEngineTest, getEntityPosition)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    const auto actualPosition = physicsEngine.getPosition(entityId);

    ASSERT_EQ(*actualPosition, position2);
}

TEST_F(DefaultPhysicsEngineTest, setEntityPosition)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    physicsEngine.setPosition(entityId, position1);
    const auto actualPosition = physicsEngine.getPosition(entityId);

    ASSERT_EQ(*actualPosition, position1);
}

TEST_F(DefaultPhysicsEngineTest, getMovementSpeed_shouldReturnInitialZeroValue)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    const auto actualMovementSpeed = physicsEngine.getMovementSpeed(entityId);

    ASSERT_EQ(*actualMovementSpeed, initialMovementSpeed);
}

TEST_F(DefaultPhysicsEngineTest, setMovementSpeed)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    physicsEngine.setMovementSpeed(entityId, movementSpeed);
    const auto actualMovementSpeed = physicsEngine.getMovementSpeed(entityId);

    ASSERT_EQ(*actualMovementSpeed, movementSpeed);
}

TEST_F(DefaultPhysicsEngineTest, getMovementDirection_shouldReturnInitialZeroValue)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    const auto actualMovementDirection = physicsEngine.getMovementDirection(entityId);

    ASSERT_EQ(*actualMovementDirection, initialMovementDirection);
}

TEST_F(DefaultPhysicsEngineTest, setMovementDirection)
{
    const auto entityId = physicsEngine.acquire(size2, position2);

    physicsEngine.setMovementDirection(entityId, movementDirection);
    const auto actualMovementDirection = physicsEngine.getMovementDirection(entityId);

    ASSERT_EQ(*actualMovementDirection, movementDirection);
}