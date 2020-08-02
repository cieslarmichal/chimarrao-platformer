#include "TextureStorageSfml.h"

#include <iostream>

namespace graphics
{
TextureStorageSfml::TextureStorageSfml(std::unique_ptr<TextureLoader> loader) : loader(std::move(loader)) {}

boost::optional<sf::Texture&> TextureStorageSfml::getTexture(const TexturePath& path)
{
    if (not inTextureMap(path))
    {
        if (not loadTexture(path))
        {
            return boost::none;
        }
    }
    return *textures[path];
}
bool TextureStorageSfml::loadTexture(const TexturePath& path)
{
    auto texture = std::make_unique<sf::Texture>();
    try
    {
        loader->load(*texture, path);
    }
    catch (const TextureLoader::CannotAccess& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    textures[path] = std::move(texture);
    return true;
}

bool TextureStorageSfml::inTextureMap(const TexturePath& path)
{
    return textures.count(path) > 0;
}
}