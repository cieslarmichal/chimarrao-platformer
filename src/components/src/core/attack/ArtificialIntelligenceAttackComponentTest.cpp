#include "ArtificialIntelligenceAttackComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "FriendlyFireValidatorMock.h"
#include "InputMock.h"
#include "RayCastMock.h"
#include "RendererPoolMock.h"

#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class ArtificialIntelligenceAttackComponentTest : public Test
{
public:
    ArtificialIntelligenceAttackComponentTest()
    {
        componentOwner.addComponent<AnimationComponent>(animator);
        componentOwner.addComponent<VelocityComponent>(6);
        componentOwner.addComponent<DirectionComponent>();
        componentOwner.addComponent<AnimationComponent>(animator);
        componentOwner.addComponent<BoxColliderComponent>(size);
        componentOwner.loadDependentComponents();
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
    std::shared_ptr<StrictMock<physics::RayCastMock>> rayCast =
        std::make_shared<StrictMock<physics::RayCastMock>>();
    std::unique_ptr<StrictMock<FriendlyFireValidatorMock>> friendlyFireValidatorInit{
        std::make_unique<StrictMock<FriendlyFireValidatorMock>>()};
    StrictMock<FriendlyFireValidatorMock>* friendlyFireValidator{friendlyFireValidatorInit.get()};
    std::shared_ptr<MeleeAttack> meleeAttack =
        std::make_shared<MeleeAttack>(&componentOwner, rayCast, std::move(friendlyFireValidatorInit));
    physics::RayCastResult rayCastResult;
};

TEST_F(ArtificialIntelligenceAttackComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    ArtificialIntelligenceAttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider,
                                                                            &targetInRange, meleeAttack};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ArtificialIntelligenceAttackComponentTest, givenTargetOutOfRange_shouldNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetOutOfRange, meleeAttack};
    attackComponent.loadDependentComponents();

    attackComponent.update(deltaTime, input);
}

TEST_F(ArtificialIntelligenceAttackComponentTest,
       givenTargetInRangeAndAnimationIsAlreadyAttack_shouldNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, meleeAttack};
    attackComponent.loadDependentComponents();
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Attack));

    attackComponent.update(deltaTime, input);
}

TEST_F(
    ArtificialIntelligenceAttackComponentTest,
    givenSpacePressedAndAnimationIsDifferentThanAttackAndAttackAnimationProgressInLessThan60Percents_shouldSetAnimationToAttackAndNotCallAttackStrategy)
{
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, meleeAttack};
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
    ArtificialIntelligenceAttackComponent attackComponent{&componentOwner, &targetInRange, meleeAttack};
    attackComponent.loadDependentComponents();
    EXPECT_CALL(*animator, getAnimationType())
        .WillOnce(Return(animations::AnimationType::Idle))
        .WillOnce(Return(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Attack));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(63));
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*rayCast, cast(_, _, _, _)).WillOnce(Return(rayCastResult));

    attackComponent.update(deltaTime, input);
}
