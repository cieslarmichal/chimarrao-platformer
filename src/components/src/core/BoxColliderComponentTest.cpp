#include "BoxColliderComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "AnimationComponent.h"
#include "DeltaTime.h"
#include "KeyboardAnimatedMovementComponent.h"

using namespace components::core;
using namespace ::testing;

class BoxColliderComponentTest : public Test
{
public:
    BoxColliderComponentTest()
    {
        componentOwnerWithMovementComponent.addComponent<AnimationComponent>(animator);
        velocityComponent = componentOwnerWithMovementComponent.addComponent<VelocityComponent>(6);
        movementComponent =
            componentOwnerWithMovementComponent.addComponent<KeyboardAnimatedMovementComponent>();
        boxColliderComponentWithMovement = std::make_shared<BoxColliderComponent>(
            &componentOwnerWithMovementComponent, size, CollisionLayer::Default, utils::Vector2f{0, 0},
            movementComponent);
        boxColliderComponentWithMovement->loadDependentComponents();
    }

    bool canMoveLeft() const
    {
        return movementComponent->isAllowedToMoveLeft();
    }

    bool canMoveRight() const
    {
        return movementComponent->isAllowedToMoveRight();
    }

    bool canMoveUp() const
    {
        return movementComponent->isAllowedToMoveUp();
    }

    bool canMoveDown() const
    {
        return movementComponent->isAllowedToMoveDown();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position{20, 20};
    const utils::Vector2f position2{16, 16};
    const utils::Vector2f positionIntersectingFromLeft{14.5, 20};
    const utils::Vector2f positionIntersectingFromRight{25.5, 20};
    const utils::Vector2f positionIntersectingFromAbove{20, 14.5};
    const utils::Vector2f positionIntersectingFromBelow{20, 25.5};
    const utils::Vector2f positionInsideTarget{20, 20};
    const utils::Vector2f positionOutsideTarget{100, 212};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwnerWithoutMovementComponent{position2, "componentOwnerWithoutMovementComponent",
                                                          sharedContext};
    BoxColliderComponent boxColliderComponentWithoutMovement{&componentOwnerWithoutMovementComponent, size,
                                                             CollisionLayer::Default};

    ComponentOwner componentOwnerWithMovementComponent{position, "componentOwnerWithMovementComponent",
                                                       sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentWithMovement;
    std::shared_ptr<KeyboardAnimatedMovementComponent> movementComponent;
    std::shared_ptr<VelocityComponent> velocityComponent;

    ComponentOwner componentOwnerNotIntersecting{positionOutsideTarget, "componentOwnerNotIntersecting",
                                                 sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentNotIntersecting =
        std::make_shared<BoxColliderComponent>(&componentOwnerNotIntersecting, size);

    ComponentOwner componentOwnerIntersectingFromLeft{positionIntersectingFromLeft,
                                                      "componentOwnerIntersectingFromLeft", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromLeft =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromLeft, size);

    ComponentOwner componentOwnerIntersectingFromRight{positionIntersectingFromRight,
                                                       "componentOwnerIntersectingFromRight", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromRight =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromRight, size);

    ComponentOwner componentOwnerIntersectingFromAbove{positionIntersectingFromAbove,
                                                       "componentOwnerIntersectingFromAbove", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponentIntersectingFromAbove =
        std::make_shared<BoxColliderComponent>(&componentOwnerIntersectingFromAbove, size);

    ComponentOwner componentOwnerIntersectingFromBelow{positionIntersectingFromBelow,
                                                       "componentOwnerIntersectingFromBelow", sharedContext};
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

TEST_F(BoxColliderComponentTest,
       givenMovingRectWithNextFrameColisionBoundariesXAndYOutsideTarget_shouldNotIntersect)
{
    ASSERT_FALSE(boxColliderComponentWithMovement->intersectsX(boxColliderComponentNotIntersecting));
    ASSERT_FALSE(boxColliderComponentWithMovement->intersectsY(boxColliderComponentNotIntersecting));
}

TEST_F(BoxColliderComponentTest, givenMovingRectWithNextFrameColisionBoundariesXInsideTarget_shouldIntersect)
{
    velocityComponent->setVelocity(-1.f, 0.f);
    boxColliderComponentWithMovement->update(deltaTime, input);

    ASSERT_TRUE(boxColliderComponentWithMovement->intersectsX(boxColliderComponentIntersectingFromLeft));
    ASSERT_FALSE(boxColliderComponentWithMovement->intersectsY(boxColliderComponentIntersectingFromLeft));
}

TEST_F(BoxColliderComponentTest, givenMovingRectWithNextFrameColisionBoundariesYInsideTarget_shouldIntersect)
{
    velocityComponent->setVelocity(0.f, 1.f);
    boxColliderComponentWithMovement->update(deltaTime, input);

    ASSERT_FALSE(boxColliderComponentWithMovement->intersectsX(boxColliderComponentIntersectingFromBelow));
    ASSERT_TRUE(boxColliderComponentWithMovement->intersectsY(boxColliderComponentIntersectingFromBelow));
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromLeft_shouldBlockLeftMovement)
{
    velocityComponent->setVelocity(-1.f, 0.f);
    boxColliderComponentWithMovement->update(deltaTime, input);
    const auto intersectsOnX =
        boxColliderComponentWithMovement->intersectsX(boxColliderComponentIntersectingFromLeft);

    boxColliderComponentWithMovement->resolveOverlapX(boxColliderComponentIntersectingFromLeft);

    ASSERT_TRUE(intersectsOnX);
    ASSERT_FALSE(canMoveLeft());
    ASSERT_TRUE(canMoveRight() && canMoveDown() && canMoveUp());
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromRight_shouldBlockRightMovement)
{
    velocityComponent->setVelocity(1.f, 0.f);
    boxColliderComponentWithMovement->update(deltaTime, input);
    const auto intersectsOnX =
        boxColliderComponentWithMovement->intersectsX(boxColliderComponentIntersectingFromRight);

    boxColliderComponentWithMovement->resolveOverlapX(boxColliderComponentIntersectingFromRight);

    ASSERT_TRUE(intersectsOnX);
    ASSERT_FALSE(canMoveRight());
    ASSERT_TRUE(canMoveLeft() && canMoveDown() && canMoveUp());
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromAbove_shouldBlockUpMovement)
{
    velocityComponent->setVelocity(0.f, -1.f);
    boxColliderComponentWithMovement->update(deltaTime, input);
    const auto intersectsOnY =
        boxColliderComponentWithMovement->intersectsY(boxColliderComponentIntersectingFromAbove);

    boxColliderComponentWithMovement->resolveOverlapY(boxColliderComponentIntersectingFromAbove);

    ASSERT_TRUE(intersectsOnY);
    ASSERT_FALSE(canMoveUp());
    ASSERT_TRUE(canMoveLeft() && canMoveRight() && canMoveDown());
}

TEST_F(BoxColliderComponentTest, resolveOverlapWithCollisionFromBelow_shouldBlockDownMovement)
{
    velocityComponent->setVelocity(0.f, 1.f);
    boxColliderComponentWithMovement->update(deltaTime, input);
    const auto intersectsOnY =
        boxColliderComponentWithMovement->intersectsY(boxColliderComponentIntersectingFromBelow);

    boxColliderComponentWithMovement->resolveOverlapY(boxColliderComponentIntersectingFromBelow);

    ASSERT_TRUE(intersectsOnY);
    ASSERT_FALSE(canMoveDown());
    ASSERT_TRUE(canMoveLeft() && canMoveRight() && canMoveUp());
}

TEST_F(BoxColliderComponentTest, getSize)
{
    ASSERT_EQ(boxColliderComponentWithMovement->getSize(), size);
}
