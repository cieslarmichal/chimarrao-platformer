#pragma once

#include <mutex>

#include "BoxColliderComponent.h"
#include "DefaultFriendlyFireValidator.h"
#include "DirectionComponent.h"
#include "RayCast.h"
#include "Timer.h"

namespace components::core
{
class MeleeAttack
{
public:
    MeleeAttack(ComponentOwner* owner, std::shared_ptr<physics::RayCast>,
                std::unique_ptr<FriendlyFireValidator>);

    void attack();

private:
    void loadDependentComponents();

    ComponentOwner* owner;
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float range{4};
    unsigned int damage{10};
    std::once_flag componentsInitialized;
    std::unique_ptr<FriendlyFireValidator> friendlyFireValidator;
};
}