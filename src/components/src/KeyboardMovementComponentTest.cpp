#include "KeyboardMovementComponent.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "animation/AnimatorMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace ::testing;
using namespace input;
using namespace components;
using namespace graphics;
using namespace animation;

class KeyboardMovementComponentTest_Base : public Test
{
public:
    KeyboardMovementComponentTest_Base()
    {
        EXPECT_CALL(*inputManager, registerObserver(_));
    }

    std::shared_ptr<StrictMock<InputManagerMock>> inputManager =
        std::make_shared<StrictMock<InputManagerMock>>();
};

class KeyboardMovementComponentTest : public KeyboardMovementComponentTest_Base
{
public:
    KeyboardMovementComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        keyboardMovementComponent.loadDependentComponents();
    }

    InputStatus prepareInputStatus(InputKey inputKey)
    {
        InputStatus inputStatus;
        inputStatus.setKeyPressed(inputKey);
        return inputStatus;
    }

    void expectRightKeyPressed()
    {
        const auto rightKeyInputStatus = prepareInputStatus(InputKey::Right);
        EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
        EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
        keyboardMovementComponent.handleInputStatus(rightKeyInputStatus);
    }

    void expectLeftKeyPressed()
    {
        const auto leftKeyInputStatus = prepareInputStatus(InputKey::Left);
        EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
        EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
        keyboardMovementComponent.handleInputStatus(leftKeyInputStatus);
    }

    const float movementSpeed{10.0};
    utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    ComponentOwner componentOwner{position};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();

    KeyboardMovementComponent keyboardMovementComponent{&componentOwner, inputManager};
};

TEST_F(KeyboardMovementComponentTest, givenInputStatusWithNoKeyPressed_shouldSetAnimationTypeToIdle)
{
    const auto noKeyInputStatus = InputStatus{};
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    keyboardMovementComponent.handleInputStatus(noKeyInputStatus);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithRightKeyPressed_shouldSetAnimationDirectionToRightAndSetAnimationTypeToWalk)
{
    const auto rightKeyInputStatus = prepareInputStatus(InputKey::Right);
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.handleInputStatus(rightKeyInputStatus);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithLeftKeyPressed_shouldSetAnimationDirectionToLeftAndSetAnimationTypeToWalk)
{
    const auto leftKeyInputStatus = prepareInputStatus(InputKey::Left);
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.handleInputStatus(leftKeyInputStatus);
}

TEST_F(KeyboardMovementComponentTest, setMovementSpeed)
{
    keyboardMovementComponent.setMovementSpeed(movementSpeed);

    ASSERT_EQ(keyboardMovementComponent.getMovementSpeed(), movementSpeed);
}

TEST_F(KeyboardMovementComponentTest,
       givenRightKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    expectRightKeyPressed();
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime);

    const auto positionChangeToRight = deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToRight, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardMovementComponentTest,
       givenLeftKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    expectLeftKeyPressed();
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime);

    const auto positionChangeToLeft = -deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToLeft, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}