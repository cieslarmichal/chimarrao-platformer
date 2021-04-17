#pragma once

#include <functional>

#include "Component.h"
#include "HitBoxComponent.h"

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

private:
    std::shared_ptr<HitBoxComponent> hitBox;
    std::function<void(void)> mouseOverAction;
    std::function<void(void)> mouseOutAction;
    bool mouseOver;
};
}
