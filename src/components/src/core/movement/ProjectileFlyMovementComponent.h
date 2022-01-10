#pragma once

#include "MovementComponent.h"
#include "Vector.h"
#include "VelocityComponent.h"
#include "AnimationComponent.h"

namespace components::core
{
class ProjectileFlyMovementComponent : public MovementComponent
{
public:
    ProjectileFlyMovementComponent(ComponentOwner*, animations::AnimationDirection direction);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
    animations::AnimationDirection direction;
};
}