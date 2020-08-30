#pragma once

#include "TexturePath.h"

namespace graphics
{
class TextureStorage
{
public:
    virtual ~TextureStorage() = default;

    virtual const sf::Texture& getTexture(const TexturePath&) = 0;
};
}