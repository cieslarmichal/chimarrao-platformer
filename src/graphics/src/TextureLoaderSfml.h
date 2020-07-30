#pragma once

#include "TextureLoader.h"

namespace graphics
{
class TextureLoaderSfml : public TextureLoader
{
    void load(sf::Texture&, const TexturePath&) override;
};
}