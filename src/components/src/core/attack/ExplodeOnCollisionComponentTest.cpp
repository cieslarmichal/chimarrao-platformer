#include "ExplodeOnCollisionComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "DirectionComponent.h"
#include "exceptions/DependentComponentNotFound.h"
#include "health/HealthComponent.h"

using namespace ::testing;
using namespace components::core;

class ExplodeOnCollisionComponentTest : public Test
{
public:
    ExplodeOnCollisionComponentTest()
    {
        componentOwner1.addComponent<VelocityComponent>(6);
        componentOwner1.addComponent<DirectionComponent>();
        componentOwner1.addComponent<AnimationComponent>(animator);
        boxColliderComponent1 = componentOwner1.addComponent<BoxColliderComponent>(size);
        componentOwner1.loadDependentComponents();

        componentOwnerOnRightInRange.addComponent<VelocityComponent>(6);
        componentOwnerOnRightInRange.addComponent<DirectionComponent>();
        componentOwnerOnRightInRange.addComponent<AnimationComponent>(animator);
        targetHealthComponentInRange = componentOwnerOnRightInRange.addComponent<HealthComponent>(100);
        componentOwnerOnRightInRange.addComponent<BoxColliderComponent>(size);
        componentOwnerOnRightInRange.loadDependentComponents();

        componentOwnerOnRightOutOfRange.addComponent<VelocityComponent>(6);
        componentOwnerOnRightOutOfRange.addComponent<DirectionComponent>();
        componentOwnerOnRightOutOfRange.addComponent<AnimationComponent>(animator);
        targetHealthComponentOutOfRange = componentOwnerOnRightOutOfRange.addComponent<HealthComponent>(100);
        componentOwnerOnRightOutOfRange.addComponent<BoxColliderComponent>(size);
        componentOwnerOnRightOutOfRange.loadDependentComponents();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInRange{24, 20};
    const utils::Vector2f positionOutOfRange{42, 20};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{1};
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner1{position1, "AttackComponentTest1", sharedContext};
    ComponentOwner componentOwnerOnRightInRange{positionInRange, "AttackComponentTest2", sharedContext};
    ComponentOwner componentOwnerOnRightOutOfRange{positionOutOfRange, "AttackComponentTest3", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1;
    std::shared_ptr<HealthComponent> targetHealthComponentInRange;
    std::shared_ptr<HealthComponent> targetHealthComponentOutOfRange;
    ExplodeOnCollisionComponent explodeOnCollisionComponent{&componentOwner1, 10};
};

TEST_F(
    ExplodeOnCollisionComponentTest,
    givenComponentOwnerWithoutAnimationComponent_loadDependentComponents_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position1, "componentOwnerWithoutDirection", sharedContext};
    ExplodeOnCollisionComponent explodeOnCollisionComponentWithoutAnimator{&componentOwnerWithoutDirection,
                                                                           5};

    ASSERT_THROW(explodeOnCollisionComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(
    ExplodeOnCollisionComponentTest,
    givenComponentOwnerWithoutBoxColliderComponent_loadDependentComponents_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>(6);
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    ExplodeOnCollisionComponent explodeOnCollisionComponentWithoutBoxCollider{
        &componentOwnerWithoutBoxCollider, 5};

    ASSERT_THROW(explodeOnCollisionComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ExplodeOnCollisionComponentTest, givenTargetInAttackRangeAndAnimationFinished_shouldDealDamageToTarget)
{
    explodeOnCollisionComponent.loadDependentComponents();
    boxColliderComponent1->setColliderOnXAxis(&componentOwnerOnRightInRange);
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Explode));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(100));

    explodeOnCollisionComponent.update(deltaTime, input);

    ASSERT_EQ(targetHealthComponentInRange->getCurrentHealth(), 90);
}

TEST_F(ExplodeOnCollisionComponentTest,
       givenTargetInAttackRangeAndAnimationNotFinished_shouldNotDealDamageToTarget)
{
    explodeOnCollisionComponent.loadDependentComponents();
    boxColliderComponent1->setColliderOnXAxis(&componentOwnerOnRightInRange);
    EXPECT_CALL(*animator, setAnimation(animations::AnimationType::Explode));
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(50));

    explodeOnCollisionComponent.update(deltaTime, input);

    ASSERT_EQ(targetHealthComponentInRange->getCurrentHealth(), 100);
}

TEST_F(ExplodeOnCollisionComponentTest, givenTargetOutOfAttackRange_shouldNotDealAnyDamageToTarget)
{
    explodeOnCollisionComponent.loadDependentComponents();

    explodeOnCollisionComponent.update(deltaTime, input);

    ASSERT_EQ(targetHealthComponentOutOfRange->getCurrentHealth(), 100);
}
