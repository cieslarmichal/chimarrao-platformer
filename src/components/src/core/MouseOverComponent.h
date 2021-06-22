#pragma once

#include <functional>

#include "BoxColliderComponent.h"
#include "Component.h"

namespace components::core
{
class MouseOverComponent : public Component
{
public:
    MouseOverComponent(ComponentOwner*, std::function<void(void)> mouseOverAction,
                       std::function<void(void)> mouseOutAction);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void enable() override;
    void disable() override;
    void invokeMouseOverAction();
    void invokeMouseOutAction();

private:
    std::shared_ptr<BoxColliderComponent> boxCollider;
    std::function<void(void)> mouseOverAction;
    std::function<void(void)> mouseOutAction;
    bool mouseOver;
};
}
