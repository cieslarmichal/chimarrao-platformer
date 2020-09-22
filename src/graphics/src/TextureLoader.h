#pragma once

#include "SFML/Graphics/Texture.hpp"

#include "TexturePath.h"

namespace graphics
{
class TextureLoader
{
public:
    static void load(sf::Texture&, const TexturePath&);
};
}