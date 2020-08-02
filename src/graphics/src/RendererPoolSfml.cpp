#include "RendererPoolSfml.h"

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
                                   std::unique_ptr<TextureStorage> textureStorageInit,
                                   std::unique_ptr<GraphicsIdGenerator> graphicsIdGeneratorInit)
    : contextRenderer{std::move(contextRendererInit)},
      textureStorage{std::move(textureStorageInit)},
      idGenerator{std::move(graphicsIdGeneratorInit)}
{
    contextRenderer->initialize();
    contextRenderer->setView();
}

GraphicsId RendererPoolSfml::acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                                     const Color& color)
{
    auto id = idGenerator->generateId();
    shapes.emplace_back(id, size, position, color);
    return id;
}

GraphicsId RendererPoolSfml::acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                                     const TexturePath& path)
{
    const auto id = acquire(size, position, Color::White);
    setTexture(id, path);
    return id;
}

void RendererPoolSfml::release(const GraphicsId& id)
{
    shapesToRemove.emplace(id);
}

void RendererPoolSfml::renderAll()
{
    cleanUnusedShapes();
    renderer->clear(sf::Color::White);
    renderer->setView();

    for (const auto& shape : shapes)
    {
        renderer->draw(shape);
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
}

void RendererPoolSfml::setTexture(const GraphicsId& id, const TexturePath& path)
{
    boost::optional<sf::Texture&> textureOpt = textureStorage->getTexture(path);
    if (not textureOpt)
    {
        return;
    }

    auto shapeIter = findShapePosition(id);
    if (shapeIter != shapes.end())
    {
        auto& shape = getShapeByPosition(shapes, shapeIter);
        shape.setTexture(*texture);
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
