#include "KeyboardMovementComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace ::testing;
using namespace input;
using namespace components::core;
using namespace animations;

class KeyboardMovementComponentTest : public Test
{
public:
    KeyboardMovementComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        keyboardMovementComponent.loadDependentComponents();
    }

    const float movementSpeed{10.0};
    utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    ComponentOwner componentOwner{position, "keyboardMovementComponentTest"};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    std::unique_ptr<StrictMock<input::InputMock>> inputInit{std::make_unique<StrictMock<input::InputMock>>()};
    StrictMock<input::InputMock>* input{inputInit.get()};

    KeyboardMovementComponent keyboardMovementComponent{&componentOwner};
};

TEST_F(KeyboardMovementComponentTest, givenInputStatusWithNoKeyPressed_shouldSetAnimationTypeToIdle)
{
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    keyboardMovementComponent.update(deltaTime, *input);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithRightKeyPressed_shouldSetAnimationDirectionToRightAndSetAnimationTypeToWalk)
{
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, *input);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithLeftKeyPressed_shouldSetAnimationDirectionToLeftAndSetAnimationTypeToWalk)
{
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, *input);
}

TEST_F(KeyboardMovementComponentTest, setMovementSpeed)
{
    keyboardMovementComponent.setMovementSpeed(movementSpeed);

    ASSERT_EQ(keyboardMovementComponent.getMovementSpeed(), movementSpeed);
}

TEST_F(KeyboardMovementComponentTest,
       givenRightKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, *input);

    const auto positionChangeToRight = deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToRight, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardMovementComponentTest,
       givenLeftKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, *input);

    const auto positionChangeToLeft = -deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToLeft, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}