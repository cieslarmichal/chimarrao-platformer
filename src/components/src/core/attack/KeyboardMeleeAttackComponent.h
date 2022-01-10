#pragma once

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "MeleeAttack.h"

namespace components::core
{
class KeyboardMeleeAttackComponent : public Component
{
public:
    KeyboardMeleeAttackComponent(ComponentOwner* owner, std::shared_ptr<MeleeAttack>);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;

private:
    bool attemptToAttack{false};
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<MeleeAttack> meleeAttack;
};
}
