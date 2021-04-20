#pragma once

#include <functional>

#include "Component.h"
#include "HitBoxComponent.h"
#include "KeyAction.h"

namespace components::core
{
class ClickableComponent : public Component
{
public:
    ClickableComponent(ComponentOwner*, std::function<void(void)> action);
    ClickableComponent(ComponentOwner*, const std::vector<KeyAction>&);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void enable() override;
    void disable() override;
    void setKeyActions(const std::vector<KeyAction>&);

private:
    std::shared_ptr<HitBoxComponent> hitBox;
    std::vector<KeyAction> keyActions;
};
}