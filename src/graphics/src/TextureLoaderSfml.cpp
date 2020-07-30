
#include "TextureLoaderSfml.h"

namespace graphics
{
void graphics::TextureLoaderSfml::load(sf::Texture& texture, const TexturePath& path)
{
    if (!texture.loadFromFile(path))
        throw CannotAccess("TextureLoader: Cannot load texture - " + path);
}
}