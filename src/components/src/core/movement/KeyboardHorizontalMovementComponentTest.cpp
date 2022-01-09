#include "KeyboardHorizontalMovementComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace animations;
using namespace ::testing;

class KeyboardHorizontalMovementComponentTest : public Test
{
public:
    KeyboardHorizontalMovementComponentTest()
    {
        owner.addComponent<VelocityComponent>(6);
        movementComponent.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner{position, "KeyboardHorizontalMovementComponentTest", sharedContext};
    KeyboardHorizontalMovementComponent movementComponent{&owner};
    StrictMock<input::InputMock> input;
};

TEST_F(KeyboardHorizontalMovementComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    KeyboardHorizontalMovementComponent movementComponentWithoutVelocity{&componentOwnerWithoutVelocity};

    ASSERT_THROW(movementComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardHorizontalMovementComponentTest, leftButtonPressed_shouldMoveLeft)
{
    const auto positionBefore = owner.transform->getPosition();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));

    movementComponent.update(deltaTime, input);
    movementComponent.lateUpdate(deltaTime, input);

    const auto positionAfter = owner.transform->getPosition();
    ASSERT_TRUE(positionAfter.x < positionBefore.x);
}

TEST_F(KeyboardHorizontalMovementComponentTest, rightButtonPressed_shouldMoveRight)
{
    const auto positionBefore = owner.transform->getPosition();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));

    movementComponent.update(deltaTime, input);
    movementComponent.lateUpdate(deltaTime, input);

    const auto positionAfter = owner.transform->getPosition();
    ASSERT_TRUE(positionAfter.x > positionBefore.x);
}