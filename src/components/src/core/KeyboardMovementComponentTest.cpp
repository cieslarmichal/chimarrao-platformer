#include "KeyboardMovementComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"

#include "AnimationComponent.h"
#include "DeltaTime.h"
#include "exceptions/DependentComponentNotFound.h"

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
        componentOwner.addComponent<BoxColliderComponent>(size);
        componentOwner.addComponent<DirectionComponent>();
        componentOwner.addComponent<VelocityComponent>();
        componentOwner.addComponent<AttackComponent>(rayCast);
        keyboardMovementComponent.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    const utils::Vector2f size{5, 5};
    ComponentOwner componentOwner{position, "keyboardMovementComponentTest"};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
    std::shared_ptr<physics::DefaultQuadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::DefaultRayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
    KeyboardMovementComponent keyboardMovementComponent{&componentOwner};
};

TEST_F(KeyboardMovementComponentTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator"};
    KeyboardMovementComponent keyboardMovementComponentWithoutAnimator{&componentOwnerWithoutAnimator};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>();

    ASSERT_THROW(keyboardMovementComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardMovementComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity"};
    KeyboardMovementComponent keyboardMovementComponentWithoutVelocity{&componentOwnerWithoutVelocity};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(keyboardMovementComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardMovementComponentTest,
       loadDependentComponentsWithoutAttackComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAttack{position, "componentOwnerWithoutAttack"};
    KeyboardMovementComponent keyboardMovementComponentWithoutAttack{&componentOwnerWithoutAttack};
    componentOwnerWithoutAttack.addComponent<VelocityComponent>();
    componentOwnerWithoutAttack.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(keyboardMovementComponentWithoutAttack.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardMovementComponentTest, givenInputStatusWithNoKeyPressed_shouldSetAnimationTypeToIdle)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithRightKeyPressed_shouldSetAnimationDirectionToRightAndSetAnimationTypeToWalk)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardMovementComponentTest,
       givenInputStatusWithLeftKeyPressed_shouldSetAnimationDirectionToLeftAndSetAnimationTypeToWalk)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Walk));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    keyboardMovementComponent.update(deltaTime, input);
}

TEST_F(KeyboardMovementComponentTest,
       givenRightKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenRightKeyPressedAndBlockedRightMovement_update_shouldNotMoveRight)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest,
       givenLeftKeyPressed_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenLeftKeyPressedAndBlockedLeftMovement_update_shouldNotMoveLeft)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest,
       givenUpKeyPressedAndObjectCannotMoveDown_update_shouldAddPositionChangeFromInputToTransformComponent)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenUpKeyPressedAndBlockedUpMovement_update_shouldNotMoveUp)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenRightAndDownKeysPressed_update_shouldRollAndMoveRight)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenLeftAndDownKeysPressed_update_shouldRollAndMoveLeft)
{
    keyboardMovementComponent.blockMoveDown();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));
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

TEST_F(KeyboardMovementComponentTest, givenSpacePressed_update_shouldSetAttackAnimation)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType()).WillRepeatedly(Return(AnimationType::Idle));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Attack));

    keyboardMovementComponent.update(deltaTime, input);
}
