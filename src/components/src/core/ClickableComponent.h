#pragma once

#include <functional>

#include "BoxColliderComponent.h"
#include "Component.h"
#include "KeyAction.h"

namespace components::core
{
class ClickableComponent : public Component
{
public:
    ClickableComponent(ComponentOwner*, std::function<void(void)> action, bool relative = true);
    ClickableComponent(ComponentOwner*, const std::vector<KeyAction>&, bool relative = true);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void enable() override;
    void disable() override;
    void setKeyActions(const std::vector<KeyAction>&);
    void invokeKeyAction(input::InputKey);

private:
    std::shared_ptr<BoxColliderComponent> boxCollider;
    std::vector<KeyAction> keyActions;
    const bool relative;
};
}