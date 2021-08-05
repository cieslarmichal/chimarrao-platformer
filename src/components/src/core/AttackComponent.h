#pragma once

#include "ComponentOwner.h"
#include "RayCast.h"
#include "DirectionComponent.h"
#include "BoxColliderComponent.h"

namespace components::core
{
class AttackComponent : public Component
{
public:
    AttackComponent(ComponentOwner* owner, std::shared_ptr<physics::RayCast>);

    void loadDependentComponents() override;
    void attack() const;

private:
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float range{4};
    unsigned int damage{10};
};
}
