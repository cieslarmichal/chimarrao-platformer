#pragma once

#include "Entity.h"
#include "InputObserver.h"
#include "Vector.h"
#include "GraphicsId.h"
#include "RendererPool.h"
#include <memory>

namespace game
{
class Player : public Entity, public InputObserver
{
public:
    explicit Player(const graphics::GraphicsId&, std::shared_ptr<graphics::RendererPool>);

    void update(const utils::DeltaTime&) override;
    bool isDead() override;
    void handleInputStatus(const InputStatus&) override;

private:
    graphics::GraphicsId graphicsId;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    utils::Vector2i direction;
    utils::Vector2f position;
};
}