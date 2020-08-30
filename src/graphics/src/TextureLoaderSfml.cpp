#include "TextureLoaderSfml.h"

#include "exceptions/CannotAccessTextureFile.h"

namespace graphics
{
void graphics::TextureLoaderSfml::load(sf::Texture& texture, const TexturePath& path) const
{
    if (not texture.loadFromFile(path))
    {
        throw exceptions::CannotAccessTextureFile("Cannot load texture: " + path);
    }
}
}