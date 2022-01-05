#pragma once

#include "AnimationComponent.h"
#include "AttackStrategy.h"
#include "ComponentOwner.h"

namespace components::core
{
class KeyboardAttackComponent : public Component
{
public:
    KeyboardAttackComponent(ComponentOwner* owner, std::shared_ptr<AttackStrategy>);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;

private:
    bool attemptToAttack{false};
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<AttackStrategy> attackStrategy;
};
}