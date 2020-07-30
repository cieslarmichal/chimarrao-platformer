#pragma once

#include <memory>

#include "DeltaTime.h"
#include "PhysicsComponent.h"
#include "SFML/Graphics/RenderTarget.hpp"

namespace game
{
class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void render(sf::RenderTarget*) = 0;

protected:
    std::unique_ptr<physics::PhysicsComponent> physicsComponent;
};
}