#include "GraphicsComponent.h"

#include "ComponentOwner.h"
#include "TextureRect.h"

namespace components::core
{

GraphicsComponent::GraphicsComponent(ComponentOwner* ownerInit,
                                     std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                                     const utils::Vector2f& size, const utils::Vector2f& position,
                                     const graphics::Color& color, graphics::VisibilityLayer layer,
                                     const utils::Vector2f& offset, bool relativeRendering,
                                     bool updatesPosition)
    : Component{ownerInit},
      rendererPool{std::move(rendererPoolInit)},
      visibilityLayer{layer},
      offset{offset},
      texturePath{boost::none},
      updatesPosition{updatesPosition}
{
    id = rendererPool->acquire(size, position, color, layer, relativeRendering);
}

GraphicsComponent::GraphicsComponent(ComponentOwner* owner,
                                     std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                                     const utils::Vector2f& size, const utils::Vector2f& position,
                                     const graphics::TexturePath& texturePath,
                                     graphics::VisibilityLayer layer, const utils::Vector2f& offset,
                                     bool relativeRendering)
    : Component{owner},
      rendererPool{std::move(rendererPoolInit)},
      visibilityLayer{layer},
      offset{offset},
      texturePath{texturePath}
{
    id = rendererPool->acquire(size, position, texturePath, layer, relativeRendering);
}

GraphicsComponent::~GraphicsComponent()
{
    rendererPool->release(id);
}

void GraphicsComponent::lateUpdate(utils::DeltaTime, const input::Input&)
{
    if (enabled and updatesPosition)
    {
        rendererPool->setPosition(id, owner->transform->getPosition() + offset);
    }
}

const graphics::GraphicsId& GraphicsComponent::getGraphicsId()
{
    return id;
}

void GraphicsComponent::setColor(const graphics::Color& color)
{
    rendererPool->setColor(id, color);
}

void GraphicsComponent::setVisibility(graphics::VisibilityLayer layer)
{
    rendererPool->setVisibility(id, layer);
    visibilityLayer = layer;
}

utils::Vector2f GraphicsComponent::getSize() const
{
    return rendererPool->getSize(id);
}

void GraphicsComponent::setSize(const utils::Vector2f& size)
{
    rendererPool->setSize(id, size);
}

boost::optional<utils::Vector2f> GraphicsComponent::getPosition() const
{
    return rendererPool->getPosition(id);
}

void GraphicsComponent::setPosition(const utils::Vector2f& position)
{
    rendererPool->setPosition(id, position);
}

void GraphicsComponent::enable()
{
    Component::enable();
    setVisibility(visibilityLayer);
}

void GraphicsComponent::disable()
{
    Component::disable();
    rendererPool->setVisibility(id, graphics::VisibilityLayer::Invisible);
}

void GraphicsComponent::setOutline(float thickness, const sf::Color& color)
{
    rendererPool->setOutline(id, thickness, color);
}

void GraphicsComponent::setTexture(const std::string& texture)
{
    texturePath = texture;
    rendererPool->setTexture(id, graphics::TextureRect{texture});
}

boost::optional<graphics::TexturePath> GraphicsComponent::getTexturePath() const
{
    return texturePath;
}
}