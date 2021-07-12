#include "MovementComponent.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace components::core;

class MovementComponentTest : public Test
{
public:
    const float initialMovementSpeed{5.0};
    const float movementSpeed{10.0};
    const utils::Vector2f position{0.0, 11.0};
    ComponentOwner componentOwner{position, "keyboardMovementComponentTest"};
    MovementComponent movementComponent{&componentOwner, initialMovementSpeed};
};

TEST_F(MovementComponentTest, initialMovementSpeed_shouldBeTheSameAsFromConstructor)
{
    ASSERT_EQ(movementComponent.getMovementSpeed(), initialMovementSpeed);
}

TEST_F(MovementComponentTest, setMovementSpeed)
{
    movementComponent.setMovementSpeed(movementSpeed);

    ASSERT_EQ(movementComponent.getMovementSpeed(), movementSpeed);
}

TEST_F(MovementComponentTest, allowMoveRight)
{
    ASSERT_TRUE(movementComponent.isAllowedToMoveRight());
    movementComponent.blockMoveRight();
    ASSERT_FALSE(movementComponent.isAllowedToMoveRight());
    movementComponent.allowMoveRight();
    ASSERT_TRUE(movementComponent.isAllowedToMoveRight());
}

TEST_F(MovementComponentTest, allowMoveLeft)
{
    ASSERT_TRUE(movementComponent.isAllowedToMoveLeft());
    movementComponent.blockMoveLeft();
    ASSERT_FALSE(movementComponent.isAllowedToMoveLeft());
    movementComponent.allowMoveLeft();
    ASSERT_TRUE(movementComponent.isAllowedToMoveLeft());
}

TEST_F(MovementComponentTest, allowMoveUp)
{
    ASSERT_TRUE(movementComponent.isAllowedToMoveUp());
    movementComponent.blockMoveUp();
    ASSERT_FALSE(movementComponent.isAllowedToMoveUp());
    movementComponent.allowMoveUp();
    ASSERT_TRUE(movementComponent.isAllowedToMoveUp());
}

TEST_F(MovementComponentTest, allowMoveDown)
{
    ASSERT_TRUE(movementComponent.isAllowedToMoveDown());
    movementComponent.blockMoveDown();
    ASSERT_FALSE(movementComponent.isAllowedToMoveDown());
    movementComponent.allowMoveDown();
    ASSERT_TRUE(movementComponent.isAllowedToMoveDown());
}