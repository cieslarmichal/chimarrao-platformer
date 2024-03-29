#include "MeleeAttack.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "FriendlyFireValidatorMock.h"
#include "RendererPoolMock.h"

#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "DirectionComponent.h"
#include "exceptions/DependentComponentNotFound.h"
#include "health/HealthComponent.h"

using namespace ::testing;
using namespace components::core;

class MeleeAttackTest : public Test
{
public:
    MeleeAttackTest()
    {
        componentOwner1.addComponent<VelocityComponent>(6);
        componentOwner1.addComponent<DirectionComponent>();
        componentOwner1.addComponent<AnimationComponent>(animator);
        componentOwner1.addComponent<BoxColliderComponent>(size);
        componentOwner1.loadDependentComponents();
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
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner1{position1, "AttackComponentTest1", sharedContext};
    ComponentOwner componentOwnerOnRightInRange{positionOnRightInRange, "AttackComponentTest2",
                                                sharedContext};
    ComponentOwner componentOwnerOnRightOutOfRange{positionOnRightOutOfRange, "AttackComponentTest3",
                                                   sharedContext};
    ComponentOwner componentOwnerOnLeftInRange{positionOnLeftInRange, "AttackComponentTest4", sharedContext};
    ComponentOwner componentOwnerOnLeftOutOfRange{positionOnLeftOutOfRange, "AttackComponentTest5",
                                                  sharedContext};
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
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
    std::unique_ptr<StrictMock<FriendlyFireValidatorMock>> friendlyFireValidatorInit{
        std::make_unique<StrictMock<FriendlyFireValidatorMock>>()};
    StrictMock<FriendlyFireValidatorMock>* friendlyFireValidator{friendlyFireValidatorInit.get()};
    MeleeAttack meleeAttack{&componentOwner1, rayCast, std::move(friendlyFireValidatorInit)};
};

TEST_F(MeleeAttackTest,
       givenComponentOwnerWithoutDirectionComponent_attack_shouldThrowDependentComponentNotFound)
{
    std::unique_ptr<StrictMock<FriendlyFireValidatorMock>> friendlyFireValidatorInit2{
        std::make_unique<StrictMock<FriendlyFireValidatorMock>>()};
    ComponentOwner componentOwnerWithoutDirection{position1, "componentOwnerWithoutDirection", sharedContext};
    MeleeAttack meleeAttackWithoutDirection{&componentOwnerWithoutDirection, rayCast,
                                            std::move(friendlyFireValidatorInit2)};

    ASSERT_THROW(meleeAttackWithoutDirection.attack(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(MeleeAttackTest,
       givenComponentOwnerWithoutBoxColliderComponent_attack_shouldThrowDependentComponentNotFound)
{
    std::unique_ptr<StrictMock<FriendlyFireValidatorMock>> friendlyFireValidatorInit2{
        std::make_unique<StrictMock<FriendlyFireValidatorMock>>()};
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>(6);
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    MeleeAttack meleeAttackWithoutBoxCollider{&componentOwnerWithoutBoxCollider, rayCast,
                                              std::move(friendlyFireValidatorInit2)};

    ASSERT_THROW(meleeAttackWithoutBoxCollider.attack(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(MeleeAttackTest, givenTargetInAttackRangeOnRight_shouldDealDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnRightInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Right));
    EXPECT_CALL(*friendlyFireValidator,
                validate(&boxColliderComponent1->getOwner(), &boxColliderComponentOnRightInRange->getOwner()))
        .WillOnce(Return(FriendlyFireValidationResult::AttackAllowed));
    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnRightInRange->getCurrentHealth(), 90);
}

TEST_F(MeleeAttackTest,
       givenTargetInAttackRangeOnRightAndVictimIsNotAllowedToAttack_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnRightInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Right));
    EXPECT_CALL(*friendlyFireValidator,
                validate(&boxColliderComponent1->getOwner(), &boxColliderComponentOnRightInRange->getOwner()))
        .WillOnce(Return(FriendlyFireValidationResult::AttackNotAllowed));

    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnRightInRange->getCurrentHealth(), 100);
}

TEST_F(MeleeAttackTest, givenTargetOutOfAttackRangeOnRight_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnRightOutOfRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Right));

    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnRightOutOfRange->getCurrentHealth(), 100);
}

TEST_F(MeleeAttackTest, givenTargetInAttackRangeOnLeft_shouldDealDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnLeftInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*friendlyFireValidator,
                validate(&boxColliderComponent1->getOwner(), &boxColliderComponentOnLeftInRange->getOwner()))
        .WillOnce(Return(FriendlyFireValidationResult::AttackAllowed));

    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnLeftInRange->getCurrentHealth(), 90);
}

TEST_F(MeleeAttackTest,
       givenTargetInAttackRangeOnLeftAndVictimIsNotAllowedToAttack_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnLeftInRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*friendlyFireValidator,
                validate(&boxColliderComponent1->getOwner(), &boxColliderComponentOnLeftInRange->getOwner()))
        .WillOnce(Return(FriendlyFireValidationResult::AttackNotAllowed));

    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnLeftInRange->getCurrentHealth(), 100);
}

TEST_F(MeleeAttackTest, givenTargetOutOfAttackRangeOnLeft_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponentOnLeftOutOfRange);
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));

    meleeAttack.attack();

    ASSERT_EQ(targetHealthComponentOnLeftOutOfRange->getCurrentHealth(), 100);
}
