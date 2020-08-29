#pragma once

#include <boost/optional.hpp>
#include <string>

#include "SFML/Graphics.hpp"

#include "TexturePath.h"

namespace graphics
{
class TextureStorage
{
public:
    virtual ~TextureStorage() = default;

    virtual boost::optional<const sf::Texture&> getTexture(const TexturePath&) = 0;
};
}