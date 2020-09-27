#include "TextComponent.h"

#include "ComponentOwner.h"

namespace components
{

TextComponent::TextComponent(ComponentOwner* ownerInit,
                             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             const utils::Vector2f& position, const std::string& text,
                             const graphics::FontPath& fontPath, unsigned characterSize,
                             const graphics::Color& color, const utils::Vector2f& offset)
    : Component{ownerInit}, rendererPool{std::move(rendererPoolInit)}, transformOffset{offset}
{
    id = rendererPool->acquireText(position, text, fontPath, characterSize, color);
}

void TextComponent::lateUpdate(utils::DeltaTime)
{
    utils::Vector2f textPosition = owner->transform->getPosition() + transformOffset;
    rendererPool->setPosition(id, textPosition);
}

const graphics::GraphicsId& TextComponent::getGraphicsId()
{
    return id;
}

void TextComponent::setColor(const graphics::Color& color)
{
    rendererPool->setColor(id, color);
}

}