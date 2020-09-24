#pragma once

#include <SFML/Graphics/Font.hpp>

#include "FontPath.h"

namespace graphics
{
class FontStorage
{
public:
    virtual ~FontStorage() = default;

    virtual const sf::Font& getFont(const FontPath&) = 0;
};
}