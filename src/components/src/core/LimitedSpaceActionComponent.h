#pragma once

#include <functional>
#include <string>

#include "Component.h"
#include "ItemCollectorComponent.h"
#include "TextComponent.h"

namespace components::core
{
class LimitedSpaceActionComponent : public Component
{
public:
    LimitedSpaceActionComponent(ComponentOwner* owner, ComponentOwner* player,
                                std::function<void(void)> action);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;
    void setAction(std::function<void(void)> newAction);

private:
    ComponentOwner* player;
    std::function<void(void)> action;
    std::shared_ptr<TextComponent> textComponent;
    const double minimumDistanceInWhichActionCanBePerformed{6};
    bool actionPerformed{false};
};

}
