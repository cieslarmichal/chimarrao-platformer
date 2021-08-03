#include "AttackComponent.h"

#include "HealthComponent.h"

namespace components::core
{

AttackComponent::AttackComponent(ComponentOwner* owner) : Component{owner} {}

void AttackComponent::setAttemptToAttack()
{
    attemptToAttackSomething = true;
}

void AttackComponent::resetAttemptToAttack()
{
    attemptToAttackSomething = false;
}

bool AttackComponent::isThereAttemptToAttack()
{
    return attemptToAttackSomething;
}

void AttackComponent::attack(const ComponentOwner& otherOwner)
{
    if (!isThereAttemptToAttack())
    {
        return;
    }

    if (auto health = otherOwner.getComponent<HealthComponent>())
    {
        health->loseHealthPoints(damage);
    }
}

}