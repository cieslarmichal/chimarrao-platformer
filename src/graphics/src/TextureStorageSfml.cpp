#include "TextureStorageSfml.h"

#include <iostream>

#include "TextureLoader.h"
#include "exceptions/CannotAccessTextureFile.h"
#include "exceptions/TextureNotAvailable.h"

namespace graphics
{

const sf::Texture& TextureStorageSfml::getTexture(const TextureRect& textureRect)
{
    if (not textureInStorage(textureRect))
    {
        loadTexture(textureRect);
    }
    return *textures.at(textureRect);
}

void TextureStorageSfml::loadTexture(const TextureRect& textureRect)
{
    auto texture = std::make_unique<sf::Texture>();
    try
    {
        TextureLoader::load(*texture, textureRect);
    }
    catch (const exceptions::CannotAccessTextureFile& e)
    {
        std::cerr << e.what() << std::endl;
        throw exceptions::TextureNotAvailable{e.what()};
    }
    textures[textureRect] = std::move(texture);
}

bool TextureStorageSfml::textureInStorage(const TextureRect& textureRect)
{
    return textures.count(textureRect) == 1;
}
}