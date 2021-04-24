#include "BoxColliderComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace components::core;
using namespace ::testing;

class BoxColliderComponentTest : public Test
{
public:
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset1{1, 1};
    const utils::Vector2f noOffset{0, 0};
    const utils::Vector2f position{20, 20};
    const utils::Vector2f positionIntersectingFromLeft{18, 21};
    const utils::Vector2f positionIntersectingFromRight{24, 21};
    const utils::Vector2f positionIntersectingFromAbove{20, 18};
    const utils::Vector2f positionIntersectingFromBelow{20, 24};
    const utils::Vector2f positionInsideTarget{25, 25};
    const utils::Vector2f positionOutsideTarget{27, 21};
    ComponentOwner componentOwner1{position, "boxColliderComponentTest1"};
    BoxColliderComponent boxColliderComponent1{&componentOwner1, size, offset1};
    ComponentOwner componentOwner2{positionOutsideTarget, "boxColliderComponentTest2"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentNotIntersecting =
        std::make_shared<BoxColliderComponent>(&componentOwner2, size, noOffset);

    ComponentOwner componentOwnerIntersectingFromLeft{positionIntersectingFromLeft, "boxColliderComponentTest3"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromLeft =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromLeft, size, noOffset);
    ComponentOwner componentOwnerIntersectingFromRight{positionIntersectingFromRight, "boxColliderComponentTest4"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromRight =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromRight, size, noOffset);
    ComponentOwner componentOwnerIntersectingFromAbove{positionIntersectingFromAbove, "boxColliderComponentTest5"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromAbove =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromAbove, size, noOffset);
    ComponentOwner componentOwnerIntersectingFromBelow{positionIntersectingFromBelow, "boxColliderComponentTest6"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromBelow =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromBelow, size, noOffset);
};

TEST_F(BoxColliderComponentTest, givenPositionInsideTarget_shouldIntersect)
{
    ASSERT_TRUE(boxColliderComponent1.intersects(positionInsideTarget));
}

TEST_F(BoxColliderComponentTest, givenPositionOutsideTarget_shouldNotIntersect)
{
    ASSERT_FALSE(boxColliderComponent1.intersects(positionOutsideTarget));
}

TEST_F(BoxColliderComponentTest, givenRectInsideTarget_shouldIntersect)
{
    const auto collisionInfo = boxColliderComponent1.intersects(boxColliderComponentIntersectingFromLeft);

    ASSERT_TRUE(collisionInfo.colliding);
    ASSERT_EQ(*collisionInfo.other, utils::FloatRect(18.f, 21.f, 5.f, 5.f));
}

TEST_F(BoxColliderComponentTest, givenRectOutsideTarget_shouldNotIntersect)
{
    const auto collisionInfo = boxColliderComponent1.intersects(boxColliderComponentNotIntersecting);

    ASSERT_FALSE(collisionInfo.colliding);
    ASSERT_FALSE(collisionInfo.other);
}

TEST_F(BoxColliderComponentTest, resolveOverlapFromStaticTransformObject_shouldNotMove)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest7"};
    componentOwner.transform->setStaticTransform(true);
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromLeft);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_EQ(collisionBoxAfter, collisionBoxBefore);
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithNoCollision_shouldNotMove)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest8"};
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentNotIntersecting);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_EQ(collisionBoxAfter, collisionBoxBefore);
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromLeft_shouldMoveToRight)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest9"};
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromLeft);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_TRUE(collisionBoxAfter.left > collisionBoxBefore.left);
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromRight_shouldMoveToLeft)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest10"};
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromRight);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_TRUE(collisionBoxAfter.left < collisionBoxBefore.left);
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromAbove_shouldMoveDownwords)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest11"};
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromAbove);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_TRUE(collisionBoxAfter.top > collisionBoxBefore.top);
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromBelow_shouldUpwords)
{
    ComponentOwner componentOwner{position, "boxColliderComponentTest12"};
    BoxColliderComponent boxColliderComponent{&componentOwner, size, offset1};
    const auto collisionInfo = boxColliderComponent.intersects(boxColliderComponentIntersectingFromBelow);
    const auto collisionBoxBefore = boxColliderComponent.getCollisionBox();

    boxColliderComponent.resolveOverlap(collisionInfo);

    const auto collisionBoxAfter = boxColliderComponent.getCollisionBox();
    ASSERT_TRUE(collisionBoxAfter.top < collisionBoxBefore.top);
}


