#pragma once

#include "Component.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class AnimationComponent;
class BoxColliderComponent;

class KeyboardMovementComponent : public Component
{
public:
    KeyboardMovementComponent(ComponentOwner*);

    void loadDependentComponents() override;
    void update(utils::DeltaTime time, const input::Input& input1) override;
    void lateUpdate(utils::DeltaTime time, const input::Input& input) override;
    void setMovementSpeed(float speed);
    float getMovementSpeed() const;

    bool canMoveRight;
    bool canMoveLeft;
    bool canMoveUp;
    bool canMoveDown;

private:
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<VelocityComponent> velocityComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    float movementSpeed;
};
}