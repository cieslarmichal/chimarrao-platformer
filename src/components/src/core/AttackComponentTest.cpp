#include "AttackComponent.h"

#include "gtest/gtest.h"

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
        componentOwner1.addComponent<BoxColliderComponent>(size);
        attackComponent.loadDependentComponents();
        targetHealthComponent1 = componentOwner2.addComponent<HealthComponent>(100);
        targetHealthComponent2 = componentOwner3.addComponent<HealthComponent>(100);
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{26, 20};
    const utils::Vector2f position3{42, 20};
    ComponentOwner componentOwner1{position1, "AttackComponentTest1"};
    ComponentOwner componentOwner2{position2, "AttackComponentTest2"};
    ComponentOwner componentOwner3{position3, "AttackComponentTest3"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1 =
        std::make_shared<BoxColliderComponent>(&componentOwner1, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent2 =
        std::make_shared<BoxColliderComponent>(&componentOwner2, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent3 =
        std::make_shared<BoxColliderComponent>(&componentOwner3, size);
    std::shared_ptr<HealthComponent> targetHealthComponent1;
    std::shared_ptr<HealthComponent> targetHealthComponent2;
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::Quadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::RayCast>(quadtree);
    AttackComponent attackComponent{&componentOwner1, rayCast};
};

TEST_F(AttackComponentTest,
       loadDependentComponentsWithoutDirectionComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position1, "componentOwnerWithoutDirection"};
    componentOwnerWithoutDirection.addComponent<BoxColliderComponent>(size);
    AttackComponent attackComponentWithoutDirection{&componentOwnerWithoutDirection, rayCast};

    ASSERT_THROW(attackComponentWithoutDirection.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(AttackComponentTest,
       loadDependentComponentsWithoutBoxColliderComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider"};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>();
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    AttackComponent attackComponentWithoutBoxCollider{&componentOwnerWithoutBoxCollider, rayCast};

    ASSERT_THROW(attackComponentWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(AttackComponentTest, givenTargetInAttackRange_shouldDealDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponent2);

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponent1->getCurrentHealth(), 90);
}

TEST_F(AttackComponentTest, givenTargetOutOfAttackRange_shouldNotDealAnyDamageToTarget)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponent3);

    attackComponent.attack();

    ASSERT_EQ(targetHealthComponent2->getCurrentHealth(), 100);
}
