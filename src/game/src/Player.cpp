#include "Player.h"

namespace game
{
Player::Player(std::unique_ptr<physics::PhysicsComponent> physicsInit)
{
    physicsComponent = std::move(physicsInit);
}

void Player::update(const utils::DeltaTime& dt)
{
    physicsComponent->move(dt);
}

void Player::render(sf::RenderTarget* renderTarget)
{
    renderTarget->draw(physicsComponent->getRectangle());
}

void Player::handleInputStatus(const InputStatus& inputStatus)
{
    if (inputStatus.isKeyPressed(InputKey::Left))
    {

    }
}

}