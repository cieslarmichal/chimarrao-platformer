#include "Player.h"

#include <iostream>

namespace game
{

Player::Player(const graphics::GraphicsId& graphicsIdInit,
               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
               const physics::PhysicsId& physicsIdInit,
               std::shared_ptr<physics::PhysicsEngine> physicsEngineInit,
               std::unique_ptr<graphics::Animator> animatorInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      physicsId{physicsIdInit},
      physicsEngine{std::move(physicsEngineInit)},
      animator{std::move(animatorInit)}
{
    physicsEngine->setMovementSpeed(physicsId, {10.0f, 10.0f});
}

void Player::update(const utils::DeltaTime& deltaTime)
{
    animator->update(deltaTime);
    const auto position = physicsEngine->getPosition(physicsId);
    rendererPool->setPosition(graphicsId, *position);
}

bool Player::isDead()
{
    return false;
}

void Player::handleInputStatus(const InputStatus& inputStatus)
{
    utils::Vector2i newDirection{0, 0};

    if (inputStatus.isKeyPressed(InputKey::Up))
    {
        newDirection.y = -1;
    }
    else if (inputStatus.isKeyPressed(InputKey::Down))
    {
        newDirection.y = 1;
    }

    if (inputStatus.isKeyPressed(InputKey::Left))
    {
        animator->setAnimation(graphics::AnimationType::Walk, graphics::AnimationDirection::Left);
//        newDirection.x = -1;
    }
    else if (inputStatus.isKeyPressed(InputKey::Right))
    {
        animator->setAnimation(graphics::AnimationType::Walk, graphics::AnimationDirection::Right);
//        newDirection.x = 1;
    }
    else
    {
        animator->setAnimation(graphics::AnimationType::Idle);
    }

    physicsEngine->setMovementDirection(physicsId, newDirection);
}

}