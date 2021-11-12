#include "ArtificialIntelligenceAttackComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "AttackStrategyMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "core/exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class ArtificialIntelligenceAttackComponentTest : public Test
{
public:
    ArtificialIntelligenceAttackComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{22, 20};
    const utils::Vector2f position3{25, 20};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "ArtificialIntelligenceAttackComponentTest1", sharedContext};
    ComponentOwner targetInRange{position2, "ArtificialIntelligenceAttackComponentTest2", sharedContext};
    ComponentOwner targetOutOfRange{position3, "ArtificialIntelligenceAttackComponentTes3", sharedContext};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{3};
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<AttackStrategyMock>> attackStrategy =
        std::make_shared<StrictMock<AttackStrategyMock>>();
};

TEST_F(ArtificialIntelligenceAttackComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    ArtificialIntelligenceAttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider,
                                                                            &targetInRange, attackStrategy};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ArtificialIntelligenceAttackComponentTest, givenTargetOutOfRange_shouldNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetOutOfRange, attackStrategy};
    attackComponent.loadDependentComponents();

    attackComponent.update(deltaTime, input);
}

TEST_F(ArtificialIntelligenceAttackComponentTest,
       givenTargetInRangeAndAnimationIsAlreadyAttack_shouldNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, attackStrategy};
    attackComponent.loadDependentComponents();
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Attack));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    ArtificialIntelligenceAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInLessThan60Percents_shouldSetAnimationToAttackAndNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, attackStrategy};
    attackComponent.loadDependentComponents();
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(58));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    ArtificialIntelligenceAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInMoreThan60Percents_shouldSetAnimationToAttackAndCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, attackStrategy};
    attackComponent.loadDependentComponents();
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(63));
    EXPECT_CALL(*attackStrategy, attack());

    attackComponent.update(deltaTime, input);
}
