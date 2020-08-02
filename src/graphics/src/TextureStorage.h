#pragma once

#include <string>

#include <boost/optional.hpp>

#include "SFML/Graphics.hpp"

namespace graphics{
using TexturePath = std::string;

class TextureStorage
{
public:
    virtual ~TextureStorage() = default;
    virtual boost::optional<const sf::Texture&> getTexture(const TexturePath&) = 0;
};
}