#pragma once

#include <functional>

#include "Component.h"
#include "HitboxComponent.h"
#include "InputManager.h"
#include "InputObserver.h"

namespace components
{
class ClickableComponent : public Component, public input::InputObserver
{
public:
    ClickableComponent(ComponentOwner*, std::shared_ptr<input::InputManager>,
                       std::function<void(void)> action);
    ~ClickableComponent();

    void loadDependentComponents() override;
    void update(utils::DeltaTime) override;
    void handleInputStatus(const input::InputStatus&) override;
    void enable() override;
    void disable() override;

private:
    std::shared_ptr<input::InputManager> inputManager;
    const input::InputStatus* inputStatus;
    std::shared_ptr<HitboxComponent> hitbox;
    std::function<void(void)> action;
    bool clicked;
};
}