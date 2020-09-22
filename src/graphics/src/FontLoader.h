#pragma once

#include "SFML/Graphics/Font.hpp"

#include "FontPath.h"

namespace graphics
{
class FontLoader
{
public:
    static void load(sf::Font&, const FontPath&);
};
}