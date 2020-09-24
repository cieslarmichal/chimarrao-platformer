#include "TextureStorageSfml.h"

#include <iostream>

#include "TextureLoader.h"
#include "exceptions/CannotAccessTextureFile.h"
#include "exceptions/TextureNotAvailable.h"

namespace graphics
{
const sf::Texture& TextureStorageSfml::getTexture(const TexturePath& path)
{
    if (not textureInStorage(path))
    {
        loadTexture(path);
    }
    return *textures.at(path);
}

void TextureStorageSfml::loadTexture(const TexturePath& path)
{
    auto texture = std::make_unique<sf::Texture>();
    try
    {
        TextureLoader::load(*texture, path);
    }
    catch (const exceptions::CannotAccessTextureFile& e)
    {
        std::cerr << e.what() << std::endl;
        throw exceptions::TextureNotAvailable{e.what()};
    }
    textures[path] = std::move(texture);
}

bool TextureStorageSfml::textureInStorage(const TexturePath& path)
{
    return textures.count(path) == 1;
}
}