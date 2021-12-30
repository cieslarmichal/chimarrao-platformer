#pragma once

#include "ComponentOwner.h"

namespace components::core
{
class VelocityComponent : public Component
{
public:
    VelocityComponent(ComponentOwner* owner, const utils::Vector2f& velocity = utils::Vector2f{0, 0});

    void setVelocity(const utils::Vector2f& vel);
    void setVelocity(float x, float y);
    const utils::Vector2f& getVelocity() const;

private:
    void trimVelocity();

    utils::Vector2f velocity;
    utils::Vector2f maxVelocity;
};
}
