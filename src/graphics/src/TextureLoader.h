#pragma once

#include "SFML/Graphics/Texture.hpp"

#include "TextureRect.h"

namespace graphics
{
class TextureLoader
{
public:
    static void load(sf::Texture&, const TextureRect&);
};
}