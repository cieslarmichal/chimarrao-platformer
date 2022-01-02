#pragma once

#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "VelocityComponent.h"

namespace components::core
{
class EnemyFollowerComponent : public MovementComponent
{
public:
    EnemyFollowerComponent(ComponentOwner* owner, ComponentOwner* player);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;

private:
    ComponentOwner* player;
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}