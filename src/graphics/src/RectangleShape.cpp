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
    setOutlineThickness(0.1);
    setOutlineColor(Color::Black);
}

GraphicsId RectangleShape::getGraphicsId() const
{
    return id;
}
}