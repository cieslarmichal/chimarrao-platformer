#pragma once

#include <memory>

#include "Component.h"
#include "RendererPool.h"
#include "TexturePath.h"
#include "Vector.h"
#include "VisibilityLayer.h"

namespace components::core
{
class GraphicsComponent : public Component
{
public:
    GraphicsComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& size,
                      const utils::Vector2f& position, const graphics::Color&,
                      graphics::VisibilityLayer = graphics::VisibilityLayer::First,
                      const utils::Vector2f& offset = {0, 0}, bool relativeRendering = false);

    GraphicsComponent(ComponentOwner*, std::shared_ptr<graphics::RendererPool>, const utils::Vector2f& size,
                      const utils::Vector2f& position, const graphics::TexturePath&,
                      graphics::VisibilityLayer = graphics::VisibilityLayer::First,
                      const utils::Vector2f& offset = {0, 0}, bool relativeRendering = false);
    ~GraphicsComponent();

    void lateUpdate(utils::DeltaTime, const input::Input& input) override;
    const graphics::GraphicsId& getGraphicsId();
    void setColor(const graphics::Color&);
    void setVisibility(graphics::VisibilityLayer);
    void setOutline(float thickness, const sf::Color& color);
    void setTexture(const std::string& texturePath);
    utils::Vector2f getSize() const;
    void setSize(const utils::Vector2f& size);
    void enable() override;
    void disable() override;
    boost::optional<graphics::TexturePath> getTexturePath() const;

private:
    std::shared_ptr<graphics::RendererPool> rendererPool;
    graphics::GraphicsId id;
    graphics::VisibilityLayer visibilityLayer;
    const utils::Vector2f offset;
    const boost::optional<graphics::TexturePath> texturePath;
};
}
