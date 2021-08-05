#pragma once

#include "AnimationDirection.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"

namespace components::core
{
class DirectionComponent : public Component
{
public:
    explicit DirectionComponent(ComponentOwner*);

    void loadDependentComponents() override;
    animations::AnimationDirection getDirection() const;
    sf::Vector2i getHeading() const;

private:
    std::shared_ptr<VelocityComponent> velocityComponent;
};
}
