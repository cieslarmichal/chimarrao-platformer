#include "PhysicsEntity.h"

#include "gtest/gtest.h"

#include "PhysicsIdGenerator.h"

using namespace physics;
using namespace ::testing;

namespace
{
const utils::Vector2f position1{3.0f, 2.0f};
const utils::Vector2f position2{1.0f, 20.0f};
const utils::Vector2f size{200, 100};
const utils::Vector2i initialMovementDirection{0, 0};
const utils::Vector2i movementDirection{1, 1};
const utils::Vector2f initialMovementSpeed{0, 0};
const utils::Vector2f movementSpeed{10, 10};
auto physicsId = PhysicsIdGenerator::generateId();
}

class PhysicsEntityTest : public Test
{
public:
    PhysicsEntity entity{physicsId, size, position1};
};

void setPosition(const utils::Vector2f& position);
void setMovementDirection(const utils::Vector2i& direction);
void setMovementSpeed(const utils::Vector2f& movementSpeed);

TEST_F(PhysicsEntityTest, getRendererId)
{
    ASSERT_EQ(physicsId, entity.getPhysicsId());
}

TEST_F(PhysicsEntityTest, getPosition)
{
    ASSERT_EQ(position1, entity.getPosition());
}

TEST_F(PhysicsEntityTest, getSize)
{
    ASSERT_EQ(size, entity.getSize());
}

TEST_F(PhysicsEntityTest, getInitialMovementDirection_shouldReturnZero)
{
    ASSERT_EQ(initialMovementDirection, entity.getMovementDirection());
}

TEST_F(PhysicsEntityTest, getInitialMovementSpeed_shouldReturnZero)
{
    ASSERT_EQ(initialMovementSpeed, entity.getMovementSpeed());
}

TEST_F(PhysicsEntityTest, setPosition)
{
    entity.setPosition(position2);

    ASSERT_EQ(entity.getPosition(), position2);
}

TEST_F(PhysicsEntityTest, setMovementDirection)
{
    entity.setMovementDirection(movementDirection);

    ASSERT_EQ(entity.getMovementDirection(), movementDirection);
}

TEST_F(PhysicsEntityTest, setMovementSpeed)
{
    entity.setMovementSpeed(movementSpeed);

    ASSERT_EQ(entity.getMovementSpeed(), movementSpeed);
}
