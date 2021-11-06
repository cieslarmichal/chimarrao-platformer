#pragma once

#include "BoxColliderComponent.h"
#include "ComponentOwner.h"
#include "DefaultRayCast.h"
#include "DirectionComponent.h"
#include "Timer.h"

namespace components::core
{
class AttackComponent : public Component
{
public:
    AttackComponent(ComponentOwner* owner, std::shared_ptr<physics::DefaultRayCast>);

    void loadDependentComponents() override;
    void attack() const;

private:
    std::shared_ptr<physics::DefaultRayCast> rayCast;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float range{4};
    unsigned int damage{10};
};
}
