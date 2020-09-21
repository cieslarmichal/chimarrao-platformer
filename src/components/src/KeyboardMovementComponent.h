#pragma once

#include "Component.h"
#include "InputManager.h"
#include "InputObserver.h"
#include "Vector.h"

namespace components
{
class AnimationComponent;

class KeyboardMovementComponent : public Component, public input::InputObserver
{
public:
    KeyboardMovementComponent(ComponentOwner*, std::shared_ptr<input::InputManager>);
    ~KeyboardMovementComponent();

    void loadDependentComponents() override;
    void update(utils::DeltaTime) override;
    void handleInputStatus(const input::InputStatus&) override;
    void setMovementSpeed(float speed);
    float getMovementSpeed() const;

private:
    std::shared_ptr<input::InputManager> inputManager;
    std::shared_ptr<AnimationComponent> animation;
    utils::Vector2f currentMovementSpeed;
    float movementSpeed;
};
}