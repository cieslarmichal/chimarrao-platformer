#pragma once

#include "AnimationComponent.h"
#include "MovementComponent.h"

namespace components::core
{
class FreeFallMovementComponent : public MovementComponent
{
public:
    FreeFallMovementComponent(ComponentOwner* owner);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}