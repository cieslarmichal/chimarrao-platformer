#include "AttackComponent.h"

#include "gtest/gtest.h"

#include "HealthComponent.h"

using namespace ::testing;
using namespace components::core;

class AttackComponentTest : public Test
{
public:
    AttackComponentTest()
    {
        targetHealthComponent = otherComponentOwner.addComponent<HealthComponent>(100);
    }

    const utils::Vector2f position1{0.0, 10.0};
    ComponentOwner componentOwner{position1, "AttackComponentTest1"};
    AttackComponent attackComponent{&componentOwner};
    ComponentOwner otherComponentOwner{position1, "AttackComponentTest2"};
    std::shared_ptr<HealthComponent> targetHealthComponent;
};

TEST_F(AttackComponentTest, attempToAttackShouldBeSetToFalseAsDefault)
{
    ASSERT_FALSE(attackComponent.isThereAttemptToAttack());
}

TEST_F(AttackComponentTest, setAttempToAttack)
{
    attackComponent.setAttemptToAttack();

    ASSERT_TRUE(attackComponent.isThereAttemptToAttack());
}

TEST_F(AttackComponentTest, givenNoAttemptToAttack_shouldNotDealAnyDamageToTarget)
{
    attackComponent.resetAttemptToAttack();

    attackComponent.attack(otherComponentOwner);

    ASSERT_EQ(targetHealthComponent->getCurrentHealth(), 100);
}

TEST_F(AttackComponentTest, givenAttemptToAttack_shouldDealDamage)
{
    attackComponent.setAttemptToAttack();

    attackComponent.attack(otherComponentOwner);

    ASSERT_EQ(targetHealthComponent->getCurrentHealth(), 90);
}
