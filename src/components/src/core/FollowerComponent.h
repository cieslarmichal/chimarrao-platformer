#pragma once

#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "VelocityComponent.h"

namespace components::core
{
class FollowerComponent : public MovementComponent
{
public:
    FollowerComponent(ComponentOwner* owner, ComponentOwner* followedOwner);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    ComponentOwner* followedOwner;
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}