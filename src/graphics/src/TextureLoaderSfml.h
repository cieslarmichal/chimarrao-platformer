#pragma once

#include "TextureLoader.h"

namespace graphics
{
class TextureLoaderSfml : public TextureLoader
{
public:
    void load(sf::Texture&, const TexturePath&) const override;
};
}