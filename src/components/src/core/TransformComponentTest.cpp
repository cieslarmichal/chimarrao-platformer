#include "TransformComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components::core;

class TransformComponentTest : public Test
{
public:
    const float x{1.0};
    const float y{2.0};
    const utils::Vector2f position1{0.0, 10.0};
    const utils::Vector2f position2{12.0, 2.0};
    const utils::Vector2f position3{x, y};
    const utils::Vector2f sumPosition1{1.0, 12.0};
    const utils::Vector2f sumPosition2{12.0, 12.0};
    ComponentOwner componentOwner{position1};
    TransformComponent transformComponent{&componentOwner, position1};
};

TEST_F(TransformComponentTest, initialPosition_shouldBeEqualToPositionPassedToConstructor)
{
    ASSERT_EQ(transformComponent.getPosition(), position1);
}

TEST_F(TransformComponentTest, setPositionByTwoCoordinates)
{
    transformComponent.setPosition(x, y);

    ASSERT_EQ(transformComponent.getPosition(), position3);
}

TEST_F(TransformComponentTest, setPositionByVectorPosition)
{
    transformComponent.setPosition(position2);

    ASSERT_EQ(transformComponent.getPosition(), position2);
}

TEST_F(TransformComponentTest, addPositionByTwoCoordinates_shouldAddPositionToCurrentPosition)
{
    transformComponent.addPosition(x, y);

    ASSERT_EQ(transformComponent.getPosition(), sumPosition1);
}

TEST_F(TransformComponentTest, addPositionByVectorPosition_shouldAddPositionToCurrentPosition)
{
    transformComponent.addPosition(position2);

    ASSERT_EQ(transformComponent.getPosition(), sumPosition2);
}

TEST_F(TransformComponentTest, setX_shouldSetOnlyXCoordinate)
{
    transformComponent.setX(x);

    ASSERT_EQ(transformComponent.getPosition().x, x);
    ASSERT_EQ(transformComponent.getPosition().y, position1.y);
}

TEST_F(TransformComponentTest, setY_shouldSetOnlyYCoordinate)
{
    transformComponent.setY(y);

    ASSERT_EQ(transformComponent.getPosition().y, y);
    ASSERT_EQ(transformComponent.getPosition().x, position1.x);
}
