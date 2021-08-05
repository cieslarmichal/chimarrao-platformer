#include "AttackComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"

#include "HealthComponent.h"
#include "core/exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class AttackComponentTest : public Test
{
public:
    AttackComponentTest()
    {
        componentOwner1.addComponent<VelocityComponent>();
        componentOwner1.addComponent<DirectionComponent>();
        componentOwner1.addComponent<AnimationComponent>(animator);
        componentOwner1.addComponent<BoxColliderComponent>(size);
        attackComponent.loadDependentComponents();
        targetHealthComponentOnRightInRange = componentOwnerOnRightInRange.addComponent<HealthComponent>(100);
        targetHealthComponentOnRightOutOfRange =
            componentOwnerOnRightOutOfRange.addComponent<HealthComponent>(100);
        targetHealthComponentOnLeftInRange = componentOwnerOnLeftInRange.addComponent<HealthComponent>(100);
        targetHealthComponentOnLeftOutOfRange =
            componentOwnerOnLeftOutOfRange.addComponent<HealthComponent>(100);
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionOnRightInRange{26, 20};
    const utils::Vector2f positionOnRightOutOfRange{42, 20};
    const utils::Vector2f positionOnLeftInRange{17, 20};
    const utils::Vector2f positionOnLeftOutOfRange{10, 20};
    ComponentOwner componentOwner1{position1, "AttackComponentTest1"};
    ComponentOwner componentOwnerOnRightInRange{positionOnRightInRange, "AttackComponentTest2"};
    ComponentOwner componentOwnerOnRightOutOfRange{positionOnRightOutOfRange, "AttackComponentTest3"};
    ComponentOwner componentOwnerOnLeftInRange{positionOnLeftInRange, "AttackComponentTest4"};
    ComponentOwner componentOwnerOnLeftOutOfRange{positionOnLeftOutOfRange, "AttackComponentTest5"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1 =
        std::make_shared<BoxColliderComponent>(&componentOwner1, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnRightInRange =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnRightInRange, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnRightOutOfRange =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnRightOutOfRange, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnLeftInRange =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnLeftInRange, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnLeftOutOfRange =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnLeftOutOfRange, size);
    std::shared_ptr<HealthComponent> targetHealthComponentOnRightInRange;
    std::shared_ptr<HealthComponent> targetHealthComponentOnRightOutOfRange;
    std::shared_ptr<HealthComponent> targetHealthComponentOnLeftInRange;
    std::shared_ptr<HealthComponent> targetHealthComponentOnLeftOutOfRange;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::Quadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::RayCast>(quadtree);
    AttackComponent attackComponent{&componentOwner1, rayCast};
};

TEST_F(AttackComponentTest,
       loadDependentComponentsWithoutDirectionComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position1, "componentOwnerWithoutDirection"};
    AttackComponent attackComponentWithoutDirection{&componentOwnerWithoutDirection, rayCast};

    ASSERT_THROW(attackComponentWithoutDirection.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(AttackComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimation{position1, "componentOwnerWithoutAnimation"};
    componentOwnerWithoutAnimation.addComponent<VelocityComponent>();
    componentOwnerWithoutAnimation.addComponent<DirectionComponent>();
    AttackComponent attackComponentWithoutAnimation{&componentOwnerWithoutAnimation, rayCast};

    ASSERT_THROW(attackComponentWithoutAnimation.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(AttackComponentTest,
       loadDependentComponentsWithoutBoxColliderComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider"};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>();
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    componentOwnerWithoutBoxCollider.addComponent<AnimationComponent>(animator);
    AttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider, rayCast};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(AttackComponentTest, givenTargetInAttackRangeOnRight_shouldDealDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnRightInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Idle));

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponentOnRightInRange->getCurrentHealth(), 90);
}

TEST_F(AttackComponentTest, givenTargetOutOfAttackRangeOnRight_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnRightOutOfRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Right));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Idle));

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponentOnRightOutOfRange->getCurrentHealth(), 100);
}

TEST_F(AttackComponentTest, givenTargetInAttackRangeOnLeft_shouldDealDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnLeftInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Idle));

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponentOnLeftInRange->getCurrentHealth(), 90);
}

TEST_F(AttackComponentTest, givenTargetOutOfAttackRangeOnLeft_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnLeftOutOfRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animations::AnimationType::Idle));

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponentOnLeftOutOfRange->getCurrentHealth(), 100);
}
