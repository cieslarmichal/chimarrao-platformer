#include "KeyboardMeleeAttackComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "FriendlyFireValidatorMock.h"
#include "InputMock.h"
#include "RayCastMock.h"
#include "RendererPoolMock.h"

#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class KeyboardMeleeAttackComponentTest : public Test
{
public:
    KeyboardMeleeAttackComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        componentOwner.addComponent<VelocityComponent>(6);
        componentOwner.addComponent<DirectionComponent>();
        componentOwner.addComponent<AnimationComponent>(animator);
        componentOwner.addComponent<BoxColliderComponent>(size);
        attackComponent.loadDependentComponents();
        componentOwner.loadDependentComponents();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "AttackComponentTest1", sharedContext};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{3};
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<physics::RayCastMock>> rayCast =
        std::make_shared<StrictMock<physics::RayCastMock>>();
    std::unique_ptr<StrictMock<FriendlyFireValidatorMock>> friendlyFireValidatorInit{
        std::make_unique<StrictMock<FriendlyFireValidatorMock>>()};
    StrictMock<FriendlyFireValidatorMock>* friendlyFireValidator{friendlyFireValidatorInit.get()};
    std::shared_ptr<MeleeAttack> meleeAttack =
        std::make_shared<MeleeAttack>(&componentOwner, rayCast, std::move(friendlyFireValidatorInit));
    KeyboardMeleeAttackComponent attackComponent{&componentOwner, meleeAttack};
    physics::RayCastResult rayCastResult{nullptr};
};

TEST_F(KeyboardMeleeAttackComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    KeyboardMeleeAttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider,
                                                                   meleeAttack};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardMeleeAttackComponentTest, givenSpaceNotPressed_shouldNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));

    attackComponent.update(deltaTime, input);
}

TEST_F(KeyboardMeleeAttackComponentTest,
       givenSpacePressedAndAnimationIsAlreadyAttack_shouldNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Attack));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    KeyboardMeleeAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInLessThan60Percents_shouldSetAnimationToAttackAndNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(58));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    KeyboardMeleeAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInMoreThan60Percents_shouldSetAnimationToAttackAndCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(63));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*rayCast, cast(_, _, _, _)).WillOnce(Return(rayCastResult));

    attackComponent.update(deltaTime, input);
}
