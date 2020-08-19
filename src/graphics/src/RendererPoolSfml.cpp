#include "RendererPoolSfml.h"

#include <iostream>

#include "GraphicsIdGenerator.h"

namespace graphics
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

RendererPoolSfml::RendererPoolSfml(std::unique_ptr<ContextRenderer> contextRendererInit,
                                   std::unique_ptr<TextureStorage> textureStorageInit)
    : contextRenderer{std::move(contextRendererInit)}, textureStorage{std::move(textureStorageInit)}
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

void RendererPoolSfml::release(const GraphicsId& id)
{
    shapesToRemove.emplace(id);
}

void RendererPoolSfml::renderAll()
{
    contextRenderer->clear(sf::Color::White);

    if (not shapesToRemove.empty())
    {
        cleanUnusedShapes();
    }
    contextRenderer->setView();

    for (const auto& shape : shapes)
    {
        contextRenderer->draw(shape);
    }
}

void RendererPoolSfml::setPosition(const GraphicsId& id, const utils::Vector2f& newPosition)
{
    auto shapeIter = findShapePosition(id);
    if (shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setPosition(newPosition);
    }
}

utils::Vector2f RendererPoolSfml::getPosition(const GraphicsId& id)
{
    auto shapeIter = findShapePosition(id);
    if (shapeIter != shapes.end())
    {
        const auto& shape = getShapeByPosition(shapes, shapeIter);
        return shape.getPosition();
    }
    return {};
}

void RendererPoolSfml::setTexture(const GraphicsId& id, const TexturePath& path, const utils::Vector2f& scale)
{
    boost::optional<const sf::Texture&> textureOpt = textureStorage->getTexture(path);
    if (not textureOpt)
    {
        return;
    }

    auto shapeIter = findShapePosition(id);
    if (shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setTexture(&(*textureOpt));
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

void RendererPoolSfml::setRenderingSize(const utils::Vector2u& renderingSize)
{
    contextRenderer->setViewSize(renderingSize);
}

std::vector<RectangleShape>::const_iterator
RendererPoolSfml::findShapePosition(const GraphicsId& graphicsIdToFind) const
{
    return std::find_if(shapes.begin(), shapes.end(), [&graphicsIdToFind](const RectangleShape& shape) {
        return shape.getGraphicsId() == graphicsIdToFind;
    });
}

void RendererPoolSfml::cleanUnusedShapes()
{
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                                [&](const RectangleShape& shape) {
                                    return shapesToRemove.count(shape.getGraphicsId());
                                }),
                 shapes.end());
    shapesToRemove.clear();
}

}
