#pragma once

#include <mutex>

#include "BoxColliderComponent.h"
#include "ComponentOwnersManager.h"
#include "DefaultFriendlyFireValidator.h"
#include "DirectionComponent.h"
#include "RayCast.h"
#include "Timer.h"
#include "AnimatorFactory.h"

namespace components::core
{
class DistanceAttack
{
public:
    DistanceAttack(ComponentOwner* owner, const std::shared_ptr<components::core::SharedContext>&,
                   std::shared_ptr<components::core::ComponentOwnersManager>);

    void attack();

private:
    void loadDependentComponents();

    ComponentOwner* owner;
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::shared_ptr<DirectionComponent> directionComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float range{4};
    unsigned int damage{10};
    std::once_flag componentsInitialized;
    std::shared_ptr<components::core::ComponentOwnersManager> ownersManager;
    std::shared_ptr<components::core::ComponentOwner> projectile;
    std::unique_ptr<animations::AnimatorFactory> animatorFactory;
};
}