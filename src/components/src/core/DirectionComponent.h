#pragma once

#include "AnimationDirection.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"

namespace components::core
{
class DirectionComponent : public Component
{
public:
    DirectionComponent(ComponentOwner* owner);

    void loadDependentComponents() override;
    animations::AnimationDirection getDirection() const;

private:
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}
