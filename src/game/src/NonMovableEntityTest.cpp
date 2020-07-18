#include "gtest/gtest.h"
#include "NonMovableEntity.h"

using namespace game;
using namespace ::testing;

namespace
{
const float xPosition{2.0};
const float yPosition{-1.0};
const float width{10.0};
const float height{20.0};
const Vector2f position{xPosition, yPosition};
const Vector2f size{width, height};
}

class NonMovableEntityTest : public Test
{
public:
    NonMovableEntity nonMovableEntity{position, size};
};

TEST_F(NonMovableEntityTest, shouldReturnXPosition)
{
    const auto actualXPosition = nonMovableEntity.getX();

    ASSERT_EQ(actualXPosition, xPosition);
}

TEST_F(NonMovableEntityTest, shouldReturnYPosition)
{
    const auto actualYPosition = nonMovableEntity.getY();

    ASSERT_EQ(actualYPosition, yPosition);
}

TEST_F(NonMovableEntityTest, shouldReturnWidth)
{
    const auto actualWidth = nonMovableEntity.getWidth();

    ASSERT_EQ(actualWidth, width);
}

TEST_F(NonMovableEntityTest, shouldReturnHeight)
{
    const auto actualHeight = nonMovableEntity.getHeight();

    ASSERT_EQ(actualHeight, height);
}

