#pragma once

#include "BoxColliderComponent.h"
#include "ComponentOwner.h"
#include "DirectionComponent.h"
#include "RayCast.h"
#include "Timer.h"

namespace components::core
{
class MeleeAttackComponent : public Component
{
public:
    MeleeAttackComponent(ComponentOwner* owner, std::shared_ptr<physics::RayCast>);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void attack() const;

private:
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float range{4};
    unsigned int damage{10};
    bool attemptToAttack{false};
};
}
