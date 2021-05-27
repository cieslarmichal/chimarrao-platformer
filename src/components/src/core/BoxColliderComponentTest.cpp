#include "BoxColliderComponent.h"

#include "gtest/gtest.h"

#include "DeltaTime.h"
#include "AnimatorMock.h"
#include "AnimationComponent.h"
#include "VelocityComponent.h"
#include "InputMock.h"

using namespace components::core;
using namespace ::testing;

class BoxColliderComponentTest : public Test
{
public:
    BoxColliderComponentTest()
    {
        componentOwnerWithMovementComponent.addComponent<AnimationComponent>(animator);
        componentOwnerWithMovementComponent.addComponent<VelocityComponent>();
        componentOwnerWithMovementComponent.addComponent<KeyboardMovementComponent>();
        boxColliderComponentWithMovement.loadDependentComponents();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position{20, 20};
    const utils::Vector2f position2{16, 16};
    const utils::Vector2f positionIntersectingFromLeft{14.5, 20};
    const utils::Vector2f positionIntersectingFromRight{24, 20};
    const utils::Vector2f positionIntersectingFromAbove{20, 18};
    const utils::Vector2f positionIntersectingFromBelow{20, 25.5};
    const utils::Vector2f positionInsideTarget{20, 20};
    const utils::Vector2f positionOutsideTarget{100, 212};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator = std::make_shared<StrictMock<animations::AnimatorMock>>();
    ComponentOwner componentOwnerWithoutMovementComponent{position2, "boxColliderComponentTest1"};
    BoxColliderComponent boxColliderComponentWithoutMovement{&componentOwnerWithoutMovementComponent, size, CollisionLayer::Default};

    ComponentOwner componentOwnerWithMovementComponent{position, "boxColliderComponentTest20"};
    BoxColliderComponent boxColliderComponentWithMovement{&componentOwnerWithMovementComponent, size, CollisionLayer::Default};

    ComponentOwner componentOwner2{positionOutsideTarget, "boxColliderComponentTest2"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentNotIntersecting =
        std::make_shared<BoxColliderComponent>(&componentOwner2, size);

    ComponentOwner componentOwnerIntersectingFromLeft{positionIntersectingFromLeft,
                                                      "boxColliderComponentTest3"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromLeft =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromLeft, size);
    ComponentOwner componentOwnerIntersectingFromRight{positionIntersectingFromRight,
                                                       "boxColliderComponentTest4"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromRight =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromRight, size);
    ComponentOwner componentOwnerIntersectingFromAbove{positionIntersectingFromAbove,
                                                       "boxColliderComponentTest5"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromAbove =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromAbove, size);
    ComponentOwner componentOwnerIntersectingFromBelow{positionIntersectingFromBelow,
                                                       "boxColliderComponentTest6"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromBelow =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromBelow, size);
    utils::DeltaTime deltaTime{1};
};

TEST_F(BoxColliderComponentTest, getCollisionLayerWithNewObject_shouldReturnDefaultLayer)
{
    ASSERT_EQ(boxColliderComponentWithoutMovement.getCollisionLayer(), CollisionLayer::Default);
}

TEST_F(BoxColliderComponentTest, shouldSetCollisionLayer)
{
    boxColliderComponentWithoutMovement.setCollisionLayer(CollisionLayer::Player);

    ASSERT_EQ(boxColliderComponentWithoutMovement.getCollisionLayer(), CollisionLayer::Player);
}

TEST_F(BoxColliderComponentTest, givenPositionInsideTarget_shouldIntersect)
{
    ASSERT_TRUE(boxColliderComponentWithoutMovement.intersects(positionInsideTarget));
}

TEST_F(BoxColliderComponentTest, givenPositionOutsideTarget_shouldNotIntersect)
{
    ASSERT_FALSE(boxColliderComponentWithoutMovement.intersects(positionOutsideTarget));
}

TEST_F(BoxColliderComponentTest, givenRectInsideTarget_shouldIntersect)
{
    ASSERT_TRUE(boxColliderComponentWithoutMovement.intersectsX(boxColliderComponentIntersectingFromLeft));
    ASSERT_TRUE(boxColliderComponentWithoutMovement.intersectsY(boxColliderComponentIntersectingFromLeft));
}

TEST_F(BoxColliderComponentTest, givenRectOutsideTarget_shouldNotIntersect)
{
    ASSERT_FALSE(boxColliderComponentWithoutMovement.intersectsX(boxColliderComponentNotIntersecting));
    ASSERT_FALSE(boxColliderComponentWithoutMovement.intersectsY(boxColliderComponentNotIntersecting));
}

TEST_F(BoxColliderComponentTest, givenMovingRectWithNextFrameColisionBoundariesXAndYOutsideTarget_shouldNotIntersect)
{
    ASSERT_FALSE(boxColliderComponentWithMovement.intersectsX(boxColliderComponentNotIntersecting));
    ASSERT_FALSE(boxColliderComponentWithMovement.intersectsY(boxColliderComponentNotIntersecting));
}

TEST_F(BoxColliderComponentTest, givenMovingRectWithNextFrameColisionBoundariesXInsideTarget_shouldIntersect)
{
    boxColliderComponentWithMovement.getOwner().getComponent<VelocityComponent>()->setVelocity(-1.f, 0.f);
    boxColliderComponentWithMovement.update(deltaTime, input);

    ASSERT_TRUE(boxColliderComponentWithMovement.intersectsX(boxColliderComponentIntersectingFromLeft));
    ASSERT_FALSE(boxColliderComponentWithMovement.intersectsY(boxColliderComponentIntersectingFromLeft));
}

TEST_F(BoxColliderComponentTest, givenMovingRectWithNextFrameColisionBoundariesYInsideTarget_shouldIntersect)
{
    boxColliderComponentWithMovement.getOwner().getComponent<VelocityComponent>()->setVelocity(0.f, 1.f);
    boxColliderComponentWithMovement.update(deltaTime, input);

    ASSERT_FALSE(boxColliderComponentWithMovement.intersectsX(boxColliderComponentIntersectingFromBelow));
    ASSERT_TRUE(boxColliderComponentWithMovement.intersectsY(boxColliderComponentIntersectingFromBelow));
}

//TEST_F(BoxColliderComponentTest, resolveOverlapWithNoCollision_shouldNotMove)
//{
//    ComponentOwner componentOwner{position, "boxColliderComponentTest8"};
//    BoxColliderComponent boxColliderComponent{&componentOwner, size, CollisionLayer::Default, offset1};
//    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentNotIntersecting);
//    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();
//
//    boxColliderComponent.resolveOverlap(collisionInfo);
//
//    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
//    ASSERT_EQ(collisionBoxAfter, collisionBoxBefore);
//}
//
//TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromLeft_shouldMoveToRight)
//{
//    ComponentOwner componentOwner{position, "boxColliderComponentTest9"};
//    BoxColliderComponent boxColliderComponent{&componentOwner, size, CollisionLayer::Default, offset1};
//    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromLeft);
//    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();
//
//    boxColliderComponent.resolveOverlap(collisionInfo);
//
//    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
//    ASSERT_TRUE(collisionBoxAfter.left > collisionBoxBefore.left);
//}
//
//TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromRight_shouldMoveToLeft)
//{
//    ComponentOwner componentOwner{position, "boxColliderComponentTest10"};
//    BoxColliderComponent boxColliderComponent{&componentOwner, size, CollisionLayer::Default, offset1};
//    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromRight);
//    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();
//
//    boxColliderComponent.resolveOverlap(collisionInfo);
//
//    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
//    ASSERT_TRUE(collisionBoxAfter.left < collisionBoxBefore.left);
//}
//
//TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromAbove_shouldMoveDownwords)
//{
//    ComponentOwner componentOwner{position, "boxColliderComponentTest11"};
//    BoxColliderComponent boxColliderComponent{&componentOwner, size, CollisionLayer::Default, offset1};
//    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromAbove);
//    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();
//
//    boxColliderComponent.resolveOverlap(collisionInfo);
//
//    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
//    ASSERT_TRUE(collisionBoxAfter.top > collisionBoxBefore.top);
//}
//
//TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromBelow_shouldUpwords)
//{
//    ComponentOwner componentOwner{position, "boxColliderComponentTest12"};
//    BoxColliderComponent boxColliderComponent{&componentOwner, size, CollisionLayer::Default, offset1};
//    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromBelow);
//    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();
//
//    boxColliderComponent.resolveOverlap(collisionInfo);
//
//    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
//    ASSERT_TRUE(collisionBoxAfter.top < collisionBoxBefore.top);
//}
