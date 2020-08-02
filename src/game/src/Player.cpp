#include "Player.h"

#include <iostream>

namespace game
{

Player::Player(const graphics::GraphicsId& id, std::shared_ptr<graphics::RendererPool> pool)
    : graphicsId{id}, rendererPool{std::move(pool)}
{
}

void Player::update(const utils::DeltaTime& deltaTime)
{
    position.x += 10.0f * direction.x * deltaTime.count();
    position.y += 10.0f * direction.y * deltaTime.count();
    rendererPool->setPosition(graphicsId, position);
}

bool Player::isDead()
{
    return false;
}

void Player::handleInputStatus(const InputStatus& inputStatus)
{
    direction.x = direction.y = 0.0f;

    if (inputStatus.isKeyPressed(InputKey::Up))
    {
        direction.y = -1.0f;
    }
    else if (inputStatus.isKeyPressed(InputKey::Down))
    {
        direction.y = 1.0f;
    }

    if (inputStatus.isKeyPressed(InputKey::Right))
    {
        direction.x = 1.0f;
    }
    else if (inputStatus.isKeyPressed(InputKey::Left))
    {
        direction.x = -1.0f;
    }
}

}