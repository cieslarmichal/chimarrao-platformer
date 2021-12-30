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

    void update(utils::DeltaTime time, const input::Input& input) override;
};
}