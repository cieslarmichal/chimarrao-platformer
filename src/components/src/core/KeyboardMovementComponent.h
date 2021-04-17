#pragma once

#include "Component.h"
#include "Vector.h"

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

private:
    std::shared_ptr<AnimationComponent> animation;
    utils::Vector2f currentMovementSpeed;
    float movementSpeed;
};
}