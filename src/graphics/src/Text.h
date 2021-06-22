#pragma once

#include "SFML/Graphics/Text.hpp"

#include "Color.h"
#include "FontPath.h"
#include "GraphicsId.h"
#include "Vector.h"

namespace graphics
{
class Text : public sf::Text
{
public:
    Text(const GraphicsId&, const utils::Vector2f& position, const std::string& text, const sf::Font& font,
         unsigned characterSize, const Color&);

    GraphicsId getGraphicsId() const;
    std::string getText() const;

private:
    GraphicsId id;
};

inline bool operator==(const Text& lhs, const Text& rhs)
{
    return lhs.getGraphicsId() == rhs.getGraphicsId();
}
}