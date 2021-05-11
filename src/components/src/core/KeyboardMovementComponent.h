#pragma once

#include "Component.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class AnimationComponent;

class KeyboardMovementComponent : public Component
{
public:
    KeyboardMovementComponent(ComponentOwner*);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void setMovementSpeed(float speed);
    float getMovementSpeed() const;

    bool canMoveRight;
    bool canMoveLeft;
    bool canMoveUp;
    bool canMoveDown;

private:
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
    float movementSpeed;
};
}