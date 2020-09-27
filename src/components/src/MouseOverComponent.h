#pragma once

#include <functional>

#include "Component.h"
#include "HitboxComponent.h"
#include "InputManager.h"
#include "InputObserver.h"

namespace components
{
class MouseOverComponent : public Component, public input::InputObserver
{
public:
    MouseOverComponent(ComponentOwner*, std::shared_ptr<input::InputManager>,
                       std::function<void(void)> mouseOverAction, std::function<void(void)> mouseOutAction);
    ~MouseOverComponent();

    void loadDependentComponents() override;
    void update(utils::DeltaTime) override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    std::shared_ptr<input::InputManager> inputManager;
    const input::InputStatus* inputStatus;
    std::shared_ptr<HitboxComponent> hitbox;
    std::function<void(void)> mouseOverAction;
    std::function<void(void)> mouseOutAction;
    bool mouseOver;
};
}
