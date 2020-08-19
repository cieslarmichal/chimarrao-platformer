#pragma once

#include <memory>

#include "Entity.h"
#include "GraphicsId.h"
#include "InputObserver.h"
#include "PhysicsEngine.h"
#include "PhysicsId.h"
#include "PlayerAnimator.h"
#include "RendererPool.h"
#include "Vector.h"

namespace game
{
class Player : public Entity, public InputObserver
{
public:
    explicit Player(const graphics::GraphicsId&, std::shared_ptr<graphics::RendererPool>,
                    const physics::PhysicsId&, std::shared_ptr<physics::PhysicsEngine>,
                    std::unique_ptr<graphics::Animator>);

    void update(const utils::DeltaTime&) override;
    bool isDead() override;
    void handleInputStatus(const InputStatus&) override;

private:
    graphics::GraphicsId graphicsId;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    physics::PhysicsId physicsId;
    std::shared_ptr<physics::PhysicsEngine> physicsEngine;
    std::unique_ptr<graphics::Animator> animator;
};
}