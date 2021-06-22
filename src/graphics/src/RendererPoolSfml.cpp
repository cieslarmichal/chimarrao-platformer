#include "RendererPoolSfml.h"

#include <iostream>

#include "GraphicsIdGenerator.h"

namespace graphics
{
namespace
{
static auto& getLayeredShapeByPosition(std::vector<ShapeRenderingInfo>& shapes,
                                       std::vector<ShapeRenderingInfo>::const_iterator position)
{
    const auto distance =
        std::vector<ShapeRenderingInfo>::size_type(std::distance(shapes.cbegin(), position));
    return shapes.at(distance);
}

static auto& getLayeredTextByPosition(std::vector<TextRenderingInfo>& texts,
                                      std::vector<TextRenderingInfo>::const_iterator position)
{
    const auto distance = std::vector<TextRenderingInfo>::size_type(std::distance(texts.cbegin(), position));
    return texts.at(distance);
}

static auto& getLayeredTextByPosition(const std::vector<TextRenderingInfo>& layeredTexts,
                                      std::vector<TextRenderingInfo>::const_iterator position)
{
    const auto distance =
        std::vector<TextRenderingInfo>::size_type(std::distance(layeredTexts.cbegin(), position));
    return layeredTexts.at(distance);
}
}

RendererPoolSfml::RendererPoolSfml(std::unique_ptr<ContextRenderer> contextRendererInit,
                                   std::unique_ptr<TextureStorage> textureStorageInit,
                                   std::unique_ptr<FontStorage> fontStorageInit)
    : contextRenderer{std::move(contextRendererInit)},
      textureStorage{std::move(textureStorageInit)},
      fontStorage{std::move(fontStorageInit)}
{
    contextRenderer->initialize();
    contextRenderer->setView();
}

GraphicsId RendererPoolSfml::acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                                     const Color& color, VisibilityLayer layer, bool relativeRendering)
{
    auto id = GraphicsIdGenerator::generateId();
    auto layeredShape =
        ShapeRenderingInfo{layer, RectangleShape{id, size, position, color}, relativeRendering};
    layeredShapes.insert(upper_bound(layeredShapes.begin(), layeredShapes.end(), layeredShape), layeredShape);
    return id;
}

GraphicsId RendererPoolSfml::acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                                     const TexturePath& path, VisibilityLayer layer, bool relativeRendering)
{
    const auto id = acquire(size, position, Color::White, layer, relativeRendering);
    setTexture(id, TextureRect{path, std::nullopt});
    return id;
}

GraphicsId RendererPoolSfml::acquireText(const utils::Vector2f& position, const std::string& text,
                                         const FontPath& fontPath, unsigned characterSize,
                                         VisibilityLayer layer, const Color& color, bool relativeRendering)
{
    auto id = GraphicsIdGenerator::generateId();
    const auto& font = fontStorage->getFont(fontPath);
    auto layeredText =
        TextRenderingInfo{layer, Text{id, position, text, font, characterSize, color}, relativeRendering};
    layeredTexts.insert(upper_bound(layeredTexts.begin(), layeredTexts.end(), layeredText), layeredText);
    return id;
}

void RendererPoolSfml::release(const GraphicsId& id)
{
    graphicsObjectsToRemove.insert(id);
}

void RendererPoolSfml::renderAll()
{
    contextRenderer->clear(sf::Color::White);

    if (not graphicsObjectsToRemove.empty())
    {
        cleanUnusedShapes();
    }

    contextRenderer->setView();
    auto viewCenter = contextRenderer->getCenter();
    auto viewSize = contextRenderer->getViewSize();
    auto relativeOffset = viewCenter - viewSize / 2.0f;

    for (auto& layeredShape : layeredShapes)
    {
        if (layeredShape.layer != VisibilityLayer::Invisible)
        {
            if (layeredShape.relativeRendering)
            {
                layeredShape.shape.move(relativeOffset);
                contextRenderer->draw(layeredShape.shape);
                layeredShape.shape.move(-relativeOffset);
            }
            else
            {
                contextRenderer->draw(layeredShape.shape);
            }
        }
    }

    for (auto& layeredText : layeredTexts)
    {
        if (layeredText.layer != VisibilityLayer::Invisible)
        {
            if (layeredText.relativeRendering)
            {
                layeredText.text.move(relativeOffset);
                contextRenderer->draw(layeredText.text);
                layeredText.text.move(-relativeOffset);
            }
            else
            {
                contextRenderer->draw(layeredText.text);
            }
        }
    }
}

void RendererPoolSfml::setPosition(const GraphicsId& id, const utils::Vector2f& newPosition)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        auto& layeredShape = getLayeredShapeByPosition(layeredShapes, layeredShapeIter);
        layeredShape.shape.setPosition(newPosition);
        return;
    }

    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        layeredText.text.setPosition(newPosition);
    }
}

boost::optional<utils::Vector2f> RendererPoolSfml::getPosition(const GraphicsId& id)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        const auto& layeredShape = getLayeredShapeByPosition(layeredShapes, layeredShapeIter);
        return layeredShape.shape.getPosition();
    }

    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        const auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        return layeredText.text.getPosition();
    }

    return boost::none;
}

void RendererPoolSfml::setTexture(const GraphicsId& id, const TextureRect& textureRect,
                                  const utils::Vector2f& scale)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        auto& layeredShape = getLayeredShapeByPosition(layeredShapes, layeredShapeIter);
        const sf::Texture& texture = textureStorage->getTexture(textureRect);
        layeredShape.shape.setTexture(&texture, true);
        layeredShape.shape.setScale(scale);
        if (scale.x < 0)
        {
            layeredShape.shape.setOrigin(layeredShape.shape.getGlobalBounds().width / (-scale.x), 0);
        }
        else
        {
            layeredShape.shape.setOrigin(0, 0);
        }
    }
}

void RendererPoolSfml::setText(const GraphicsId& id, const std::string& text)
{
    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        layeredText.text.setString(text);
    }
}

boost::optional<std::string> RendererPoolSfml::getText(const GraphicsId& id) const
{
    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        const auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        return layeredText.text.getText();
    }

    return boost::none;
}

void RendererPoolSfml::setVisibility(const GraphicsId& id, VisibilityLayer layer)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        auto layeredShape = *layeredShapeIter;
        layeredShape.layer = layer;
        layeredShapes.erase(layeredShapeIter);
        layeredShapes.insert(upper_bound(layeredShapes.begin(), layeredShapes.end(), layeredShape),
                             layeredShape);
        return;
    }

    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        auto layeredText = *layeredTextIter;
        layeredText.layer = layer;
        layeredTexts.erase(layeredTextIter);
        layeredTexts.insert(upper_bound(layeredTexts.begin(), layeredTexts.end(), layeredText), layeredText);
    }
}

void RendererPoolSfml::setColor(const GraphicsId& id, const Color& color)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        auto& layeredShape = getLayeredShapeByPosition(layeredShapes, layeredShapeIter);
        layeredShape.shape.setFillColor(color);
        return;
    }

    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        layeredText.text.setFillColor(color);
    }
}

void RendererPoolSfml::setOutline(const GraphicsId& id, float thickness, const Color& color)
{
    if (const auto layeredShapeIter = findLayeredShapePosition(id); layeredShapeIter != layeredShapes.end())
    {
        auto& layeredShape = getLayeredShapeByPosition(layeredShapes, layeredShapeIter);
        layeredShape.shape.setOutlineThickness(thickness);
        layeredShape.shape.setOutlineColor(color);
        return;
    }

    if (const auto layeredTextIter = findLayeredTextPosition(id); layeredTextIter != layeredTexts.end())
    {
        auto& layeredText = getLayeredTextByPosition(layeredTexts, layeredTextIter);
        layeredText.text.setOutlineThickness(thickness);
        layeredText.text.setOutlineColor(color);
    }
}

void RendererPoolSfml::setRenderingSize(const utils::Vector2u& renderingSize)
{
    contextRenderer->setViewSize(renderingSize);
}

void RendererPoolSfml::synchronizeRenderingSize()
{
    contextRenderer->synchronizeViewSize();
}

void RendererPoolSfml::cleanUnusedShapes()
{
    layeredShapes.erase(
        std::remove_if(layeredShapes.begin(), layeredShapes.end(),
                       [&](const ShapeRenderingInfo& layeredShape)
                       { return graphicsObjectsToRemove.count(layeredShape.shape.getGraphicsId()); }),
        layeredShapes.end());

    layeredTexts.erase(
        std::remove_if(layeredTexts.begin(), layeredTexts.end(),
                       [&](const TextRenderingInfo& layeredText)
                       { return graphicsObjectsToRemove.count(layeredText.text.getGraphicsId()); }),
        layeredTexts.end());

    graphicsObjectsToRemove.clear();
}

std::vector<ShapeRenderingInfo>::const_iterator
RendererPoolSfml::findLayeredShapePosition(const GraphicsId& graphicsIdToFind) const
{
    return std::find_if(layeredShapes.begin(), layeredShapes.end(),
                        [&graphicsIdToFind](const ShapeRenderingInfo& layeredShape)
                        { return layeredShape.shape.getGraphicsId() == graphicsIdToFind; });
}

std::vector<TextRenderingInfo>::const_iterator
RendererPoolSfml::findLayeredTextPosition(const GraphicsId& graphicsIdToFind) const
{
    return std::find_if(layeredTexts.begin(), layeredTexts.end(),
                        [&graphicsIdToFind](const TextRenderingInfo& layeredText)
                        { return layeredText.text.getGraphicsId() == graphicsIdToFind; });
}

void RendererPoolSfml::setCenter(const utils::Vector2f& movement)
{
    contextRenderer->setCenter(movement);
}

const utils::Vector2f& RendererPoolSfml::getCenter() const
{
    return contextRenderer->getCenter();
}
const utils::Vector2f& RendererPoolSfml::getViewSize() const
{
    return contextRenderer->getViewSize();
}
}
