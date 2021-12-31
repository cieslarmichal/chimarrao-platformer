#pragma once

#include "MovementComponent.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class KeyboardHorizontalMovementComponent : public MovementComponent
{
public:
    explicit KeyboardHorizontalMovementComponent(ComponentOwner*);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}