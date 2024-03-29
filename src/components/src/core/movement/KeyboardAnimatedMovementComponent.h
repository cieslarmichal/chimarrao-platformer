#pragma once

#include "MovementComponent.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class AnimationComponent;

class KeyboardAnimatedMovementComponent : public MovementComponent
{
public:
    explicit KeyboardAnimatedMovementComponent(ComponentOwner*);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}