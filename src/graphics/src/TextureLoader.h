#pragma once

#include <string>

#include "SFML/Graphics.hpp"

namespace graphics
{
using TexturePath = std::string;

class TextureLoader
{
public:
    struct CannotAccess : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    virtual ~TextureLoader() = default;
    virtual void load(sf::Texture&, const TexturePath&) = 0;
};
}