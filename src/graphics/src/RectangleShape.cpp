#include "RectangleShape.h"

namespace graphics
{
RectangleShape::RectangleShape(GraphicsId graphicsId, const utils::Vector2f& size,
                               const utils::Vector2f& position, const Color& color)
    : id{graphicsId}
{
    setSize(size);
    setPosition(position);
    setFillColor(color);
}

utils::Vector2f RectangleShape::getPosition() const
{
    return sf::RectangleShape::getPosition();
}

utils::Vector2f RectangleShape::getSize() const
{
    return sf::RectangleShape::getSize();
}

GraphicsId RectangleShape::getGraphicsId() const
{
    return id;
}
}