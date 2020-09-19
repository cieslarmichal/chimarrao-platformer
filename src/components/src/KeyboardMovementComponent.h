#pragma once

#include "Component.h"
#include "InputObserver.h"
#include "Vector.h"
#include "InputManager.h"

namespace components
{
class AnimationComponent;

class KeyboardMovementComponent : public Component, public input::InputObserver
{
public:
    KeyboardMovementComponent(ComponentOwner*, const std::shared_ptr<input::InputManager>&);

    void awake() override;
    void update(utils::DeltaTime) override;
    void handleInputStatus(const input::InputStatus&) override;
    void setMovementSpeed(float speed);

private:
    std::shared_ptr<AnimationComponent> animation;
    utils::Vector2f currentMovementSpeed;
    float movementSpeed;
};
}