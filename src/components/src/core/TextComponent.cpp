#include "TextComponent.h"

#include "ComponentOwner.h"

namespace components::core
{

TextComponent::TextComponent(ComponentOwner* ownerInit,
                             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             const utils::Vector2f& position, const std::string& text,
                             const graphics::FontPath& fontPath, unsigned characterSize,
                             const graphics::Color& color, const utils::Vector2f& offset)
    : Component{ownerInit},
      rendererPool{std::move(rendererPoolInit)},
      transformOffset{offset},
      visibilityLayer{graphics::VisibilityLayer::First}
{
    id = rendererPool->acquireText(position, text, fontPath, characterSize, visibilityLayer, color);
}

TextComponent::~TextComponent()
{
    rendererPool->release(id);
}

void TextComponent::lateUpdate(utils::DeltaTime)
{
    if (enabled)
    {
        utils::Vector2f textPosition = owner->transform->getPosition() + transformOffset;
        rendererPool->setPosition(id, textPosition);
    }
}

const graphics::GraphicsId& TextComponent::getGraphicsId()
{
    return id;
}

void TextComponent::setColor(const graphics::Color& color)
{
    rendererPool->setColor(id, color);
}

void TextComponent::setText(const std::string& text)
{
    rendererPool->setText(id, text);
}

void TextComponent::enable()
{
    Component::enable();
    rendererPool->setVisibility(id, visibilityLayer);
}

void TextComponent::disable()
{
    Component::disable();
    rendererPool->setVisibility(id, graphics::VisibilityLayer::Invisible);
}

}