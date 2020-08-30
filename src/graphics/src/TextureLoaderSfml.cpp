#include "TextureLoaderSfml.h"
#include "exceptions/CannotAccess.h"

namespace graphics
{
void graphics::TextureLoaderSfml::load(sf::Texture& texture, const TexturePath& path) const
{
    if (not texture.loadFromFile(path))
    {
        throw exceptions::CannotAccess("Cannot load texture: " + path);
    }
}
}