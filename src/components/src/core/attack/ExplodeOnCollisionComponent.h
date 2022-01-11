#pragma once

#include <mutex>

#include "AnimatorFactory.h"
#include "BoxColliderComponent.h"
#include "ComponentOwnersManager.h"
#include "DirectionComponent.h"

namespace components::core
{
class ExplodeOnCollisionComponent : public Component
{
public:
    ExplodeOnCollisionComponent(ComponentOwner* owner, unsigned int damage);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;

private:
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::shared_ptr<AnimationComponent> animationComponent;
    std::shared_ptr<MovementComponent> movementComponent;
    const unsigned int damage;
    bool explosionStarted;
    bool explosionFinished;
    ComponentOwner* target;
};
}