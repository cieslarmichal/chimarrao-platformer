//#include "MovableEntity.h"
//
//#include "gtest/gtest.h"
//
//using namespace game;
//using namespace ::testing;
//
//namespace
//{
//const float xPosition{2.0};
//const float yPosition{-1.0};
//const float width{10.0};
//const float height{20.0};
//const float velocityX{-2.0};
//const float velocityY{1.0};
//const utils::Vector2f position{xPosition, yPosition};
//const utils::Vector2f velocity{velocityX, velocityY};
//const utils::Vector2f size{width, height};
//}
//
//class MovableEntityTest : public Test
//{
//public:
//    MovableEntity movableEntity{position, velocity, size};
//};
//
//TEST_F(MovableEntityTest, shouldReturnXPosition)
//{
//    const auto actualXPosition = movableEntity.getX();
//
//    ASSERT_EQ(actualXPosition, xPosition);
//}
//
//TEST_F(MovableEntityTest, shouldReturnYPosition)
//{
//    const auto actualYPosition = movableEntity.getY();
//
//    ASSERT_EQ(actualYPosition, yPosition);
//}
//
//TEST_F(MovableEntityTest, shouldReturnXelocity)
//{
//    const auto actualVelocity = movableEntity.getVelocity();
//
//    ASSERT_EQ(actualVelocity, velocity);
//}
//
//TEST_F(MovableEntityTest, shouldReturnWidth)
//{
//    const auto actualWidth = movableEntity.getWidth();
//
//    ASSERT_EQ(actualWidth, width);
//}
//
//TEST_F(MovableEntityTest, shouldReturnHeight)
//{
//    const auto actualHeight = movableEntity.getHeight();
//
//    ASSERT_EQ(actualHeight, height);
//}
