#pragma once

#include <memory>

#include "Component.h"
#include "RendererPool.h"
#include "Vector.h"

namespace components
{
class GraphicsComponent : public Component
{
public:
    GraphicsComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& size,
                      const utils::Vector2f& position);
    void lateUpdate(utils::DeltaTime) override;
    const graphics::GraphicsId& getGraphicsId();

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    graphics::GraphicsId id;
};
}
