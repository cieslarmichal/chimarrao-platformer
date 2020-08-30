#pragma once

#include <string>

#include "SFML/Graphics.hpp"

namespace graphics
{
using TexturePath = std::string;

class TextureLoader
{
public:
    virtual ~TextureLoader() = default;

    virtual void load(sf::Texture&, const TexturePath&) const = 0;
};
}