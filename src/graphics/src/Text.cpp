#include "Text.h"

namespace graphics
{

Text::Text(const GraphicsId& graphicsId, const utils::Vector2f& position, const std::string& text,
           const sf::Font& font, unsigned int characterSize, const Color& color)
    : id{graphicsId}
{
    setScale(0.1f, 0.1f);
    setFont(font);
    setString(text);
    setFillColor(color);
    setOutlineThickness(0.2f);
    setCharacterSize(characterSize);
    setPosition(position);
}

GraphicsId Text::getGraphicsId() const
{
    return id;
}

}