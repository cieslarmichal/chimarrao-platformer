#include "KeyboardAttackComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "AttackStrategyMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "core/exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class KeyboardAttackComponentTest : public Test
{
public:
    KeyboardAttackComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        attackComponent.loadDependentComponents();
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
    std::unique_ptr<StrictMock<AttackStrategyMock>> attackStrategyInit =
        std::make_unique<StrictMock<AttackStrategyMock>>();
    StrictMock<AttackStrategyMock>* attackStrategy = attackStrategyInit.get();
    KeyboardAttackComponent attackComponent{&componentOwner, std::move(attackStrategyInit)};
};

TEST_F(KeyboardAttackComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    KeyboardAttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider,
                                                              std::move(attackStrategyInit)};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(KeyboardAttackComponentTest, givenSpaceNotPressed_shouldNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(false));

    attackComponent.update(deltaTime, input);
}

TEST_F(KeyboardAttackComponentTest, givenSpacePressedAndAnimationIsAlreadyAttack_shouldNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Attack));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    KeyboardAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInLessThan60Percents_shouldSetAnimationToAttackAndNotCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
        EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(58));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    KeyboardAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInMoreThan60Percents_shouldSetAnimationToAttackAndCallAttackStrategy)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Space)).WillOnce(Return(true));
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(63));
    EXPECT_CALL(*attackStrategy, attack());

    attackComponent.update(deltaTime, input);
}
