#include "KeyboardAnimatedMovementComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "AnimationComponent.h"
#include "DeltaTime.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace input;
using namespace components::core;
using namespace animations;

class KeyboardAnimatedMovementComponentTest : public Test
{
public:
    KeyboardAnimatedMovementComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        componentOwner.addComponent<VelocityComponent>();
        keyboardMovementComponent.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    const utils::Vector2f size{5, 5};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position, "keyboardAnimatedMovementComponentTest", sharedContext};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
    KeyboardAnimatedMovementComponent keyboardMovementComponent{&componentOwner};
};

TEST_F(KeyboardAnimatedMovementComponentTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator", sharedContext};
    KeyboardAnimatedMovementComponent keyboardMovementComponentWithoutAnimator{
        &componentOwnerWithoutAnimator};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>();

    ASSERT_THROW(keyboardMovementComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    KeyboardAnimatedMovementComponent keyboardMovementComponentWithoutVelocity{
        &componentOwnerWithoutVelocity};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(keyboardMovementComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardAnimatedMovementComponentTest, givenInputStatusWithNoKeyPressed_shouldSetAnimationTypeToIdle)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenInputStatusWithRightKeyPressed_shouldSetAnimationDirectionToRightAndSetAnimationTypeToWalk)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenInputStatusWithLeftKeyPressed_shouldSetAnimationDirectionToLeftAndSetAnimationTypeToWalk)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenRightKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionChangeToRight = deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToRight, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenRightKeyPressedAndBlockedRightMovement_update_shouldNotMoveRight)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.blockMoveRight();
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto expectedPositionAfterUpdate = utils::Vector2f{positionBeforeUpdate.x, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenLeftKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionChangeToLeft = -deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToLeft, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenLeftKeyPressedAndBlockedLeftMovement_update_shouldNotMoveLeft)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Shift)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.blockMoveLeft();
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto expectedPositionAfterUpdate = utils::Vector2f{positionBeforeUpdate.x, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest,
       givenUpKeyPressedAndObjectCannotMoveDown_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Jump));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();
    keyboardMovementComponent.blockMoveDown();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionChangeToUp = deltaTime.count() * -17.f;
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x, positionBeforeUpdate.y + positionChangeToUp};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest, givenUpKeyPressedAndBlockedUpMovement_update_shouldNotMoveUp)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Jump));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();
    keyboardMovementComponent.blockMoveDown();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.blockMoveUp();
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, positionBeforeUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest, givenRightAndDownKeysPressed_update_shouldRollAndMoveRight)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Roll));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionChangeToRight =
        1.5f * deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToRight, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}

TEST_F(KeyboardAnimatedMovementComponentTest, givenLeftAndDownKeysPressed_update_shouldRollAndMoveLeft)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Roll));
    const auto positionBeforeUpdate = componentOwner.transform->getPosition();

    keyboardMovementComponent.update(deltaTime, input);
    keyboardMovementComponent.lateUpdate(deltaTime, input);

    const auto positionChangeToLeft =
        -1.5f * deltaTime.count() * keyboardMovementComponent.getMovementSpeed();
    const auto expectedPositionAfterUpdate =
        utils::Vector2f{positionBeforeUpdate.x + positionChangeToLeft, positionBeforeUpdate.y};
    const auto positionAfterUpdate = componentOwner.transform->getPosition();
    ASSERT_EQ(positionAfterUpdate, expectedPositionAfterUpdate);
}
