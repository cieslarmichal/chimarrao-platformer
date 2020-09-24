#include "RendererPoolSfml.h"

#include <iostream>

#include "GraphicsIdGenerator.h"

namespace graphics
{
namespace
{
static auto& getShapeByPosition(std::vector<RectangleShape>& shapes,
                                std::vector<RectangleShape>::const_iterator position)
{
    const auto distance = std::vector<RectangleShape>::size_type(std::distance(shapes.cbegin(), position));
    return shapes.at(distance);
}

static auto& getShapeByPosition(const std::vector<RectangleShape>& shapes,
                                std::vector<RectangleShape>::const_iterator position)
{
    const auto distance = std::vector<RectangleShape>::size_type(std::distance(shapes.cbegin(), position));
    return shapes.at(distance);
}

static auto& getTextByPosition(std::vector<Text>& texts, std::vector<Text>::const_iterator position)
{
    const auto distance = std::vector<Text>::size_type(std::distance(texts.cbegin(), position));
    return texts.at(distance);
}

static auto& getTextByPosition(const std::vector<Text>& texts, std::vector<Text>::const_iterator position)
{
    const auto distance = std::vector<Text>::size_type(std::distance(texts.cbegin(), position));
    return texts.at(distance);
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
                                     const Color& color)
{
    auto id = GraphicsIdGenerator::generateId();
    shapes.emplace_back(id, size, position, color);
    return id;
}

GraphicsId RendererPoolSfml::acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                                     const TexturePath& path)
{
    const auto id = acquire(size, position, Color::Red);
    setTexture(id, path);
    return id;
}

GraphicsId RendererPoolSfml::acquireText(const utils::Vector2f& position, const std::string& text,
                                         const FontPath& fontPath, unsigned characterSize, const Color& color)
{
    auto id = GraphicsIdGenerator::generateId();
    const auto& font = fontStorage->getFont(fontPath);
    texts.emplace_back(id, position, text, font, characterSize, color);
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

    for (const auto& shape : shapes)
    {
        contextRenderer->draw(shape);
    }

    for (const auto& text : texts)
    {
        contextRenderer->draw(text);
    }
}

void RendererPoolSfml::setPosition(const GraphicsId& id, const utils::Vector2f& newPosition)
{
    if (const auto shapeIter = findShapePosition(id); shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setPosition(newPosition);
        return;
    }

    if (const auto textIter = findTextPosition(id); textIter != texts.end())
    {
        auto& text = getTextByPosition(texts, textIter);
        text.setPosition(newPosition);
    }
}

boost::optional<utils::Vector2f> RendererPoolSfml::getPosition(const GraphicsId& id)
{
    if (const auto shapeIter = findShapePosition(id); shapeIter != shapes.end())
    {
        const auto& shape = getShapeByPosition(shapes, shapeIter);
        return shape.getPosition();
    }

    if (const auto textIter = findTextPosition(id); textIter != texts.end())
    {
        const auto& text = getTextByPosition(texts, textIter);
        return text.getPosition();
    }

    return boost::none;
}

void RendererPoolSfml::setTexture(const GraphicsId& id, const TexturePath& path, const utils::Vector2f& scale)
{
    const sf::Texture& texture = textureStorage->getTexture(path);

    if (const auto shapeIter = findShapePosition(id); shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setTexture(&texture);
        shape.setScale(scale);
        if (scale.x < 0)
        {
            shape.setOrigin(shape.getGlobalBounds().width / (-scale.x), 0);
        }
        else
        {
            shape.setOrigin(0, 0);
        }
    }
}

void RendererPoolSfml::setColor(const GraphicsId& id, const Color& color)
{
    if (const auto shapeIter = findShapePosition(id); shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setFillColor(color);
    }

    if (const auto textIter = findTextPosition(id); textIter != texts.end())
    {
        auto& text = getTextByPosition(texts, textIter);
        text.setFillColor(color);
    }
}

void RendererPoolSfml::setRenderingSize(const utils::Vector2u& renderingSize)
{
    // TODO: not working as it suppose
    contextRenderer->setViewSize(renderingSize);
}

void RendererPoolSfml::cleanUnusedShapes()
{
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                                [&](const RectangleShape& shape) {
                                    return graphicsObjectsToRemove.count(shape.getGraphicsId());
                                }),
                 shapes.end());

    texts.erase(
        std::remove_if(texts.begin(), texts.end(),
                       [&](const Text& text) { return graphicsObjectsToRemove.count(text.getGraphicsId()); }),
        texts.end());

    graphicsObjectsToRemove.clear();
}

std::vector<RectangleShape>::const_iterator
RendererPoolSfml::findShapePosition(const GraphicsId& graphicsIdToFind) const
{
    return std::find_if(shapes.begin(), shapes.end(), [&graphicsIdToFind](const RectangleShape& shape) {
        return shape.getGraphicsId() == graphicsIdToFind;
    });
}

std::vector<Text>::const_iterator RendererPoolSfml::findTextPosition(const GraphicsId& graphicsIdToFind) const
{
    return std::find_if(texts.begin(), texts.end(), [&graphicsIdToFind](const Text& text) {
        return text.getGraphicsId() == graphicsIdToFind;
    });
}

}
