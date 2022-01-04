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

private:
    ComponentOwner* player;
    std::function<void(void)> action;
    std::shared_ptr<TextComponent> textComponent;
    std::shared_ptr<ItemCollectorComponent> playersItemCollector;
    const double minimumDistanceInWhichActionCanBePerformed{6};
    bool actionPerformed{false};
};

}
