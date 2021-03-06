#pragma once

#include <functional>

#include "Component.h"
#include "HitBoxComponent.h"
#include "InputManager.h"
#include "InputObserver.h"
#include "KeyAction.h"

namespace components::core
{
class ClickableComponent : public Component, public input::InputObserver
{
public:
    ClickableComponent(ComponentOwner*, std::shared_ptr<input::InputManager>,
                       std::function<void(void)> action);
    ClickableComponent(ComponentOwner*, std::shared_ptr<input::InputManager>,
                       std::vector<KeyAction> keyActionVector);
    ~ClickableComponent();

    void loadDependentComponents() override;
    void update(utils::DeltaTime) override;
    void handleInputStatus(const input::InputStatus&) override;
    void enable() override;
    void disable() override;

private:
    std::shared_ptr<input::InputManager> inputManager;
    const input::InputStatus* inputStatus;
    std::shared_ptr<HitBoxComponent> hitbox;
    std::vector<KeyAction> keyActionVector;
};
}