#pragma once

#include "ComponentOwner.h"

namespace components::core
{
class AttackComponent : public Component
{
public:
    AttackComponent(ComponentOwner* owner);

    void setAttemptToAttack();
    void resetAttemptToAttack();
    bool isThereAttemptToAttack();
    void attack(const ComponentOwner&);

private:
    bool attemptToAttackSomething{false};
    unsigned int damage{10};
};
}
