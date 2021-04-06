#pragma once

#include "TexturePath.h"
#include "TextureRect.h"

namespace graphics
{
class TextureStorage
{
public:
    virtual ~TextureStorage() = default;

    virtual const sf::Texture& getTexture(const TextureRect&) = 0;
};
}