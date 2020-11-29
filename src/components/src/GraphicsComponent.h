#pragma once

#include <memory>

#include "Component.h"
#include "RendererPool.h"
#include "TexturePath.h"
#include "Vector.h"
#include "VisibilityLayer.h"

namespace components
{
class GraphicsComponent : public Component
{
public:
    GraphicsComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& size,
                      const utils::Vector2f& position, const graphics::Color&,
                      graphics::VisibilityLayer = graphics::VisibilityLayer::First);

    GraphicsComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& size,
                      const utils::Vector2f& position, const graphics::TexturePath&,
                      graphics::VisibilityLayer = graphics::VisibilityLayer::First);
    ~GraphicsComponent();

    void lateUpdate(utils::DeltaTime) override;
    const graphics::GraphicsId& getGraphicsId();
    void setColor(const graphics::Color&);
    void setVisibility(graphics::VisibilityLayer);
    void setOutline(float thickness, const sf::Color& color);
    void setTexture(const std::string& texturePath);
    void enable() override;
    void disable() override;

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    graphics::GraphicsId id;
    const graphics::VisibilityLayer visibilityLayer;
};
}
