#pragma once

#include "Entity.h"
#include "InputObserver.h"
#include "Vector.h"

namespace game
{
class Player : public Entity, public InputObserver
{
public:
    explicit Player(std::unique_ptr<physics::PhysicsComponent>);

    void update(const utils::DeltaTime&) override;
    void render(sf::RenderTarget*) override;
    void handleInputStatus(const InputStatus&) override;
};
}