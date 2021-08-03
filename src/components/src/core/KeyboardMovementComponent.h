#pragma once

#include "MovementComponent.h"
#include "Vector.h"
#include "VelocityComponent.h"
#include "AttackComponent.h"

namespace components::core
{
class AnimationComponent;

class KeyboardMovementComponent : public MovementComponent
{
public:
    KeyboardMovementComponent(ComponentOwner*);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
    std::shared_ptr<AttackComponent> attackComponent;
};
}