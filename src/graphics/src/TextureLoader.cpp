#include "TextureLoader.h"

#include "exceptions/CannotAccessTextureFile.h"

namespace graphics
{
void graphics::TextureLoader::load(sf::Texture& texture, const TexturePath& path)
{
    if (not texture.loadFromFile(path))
    {
        throw exceptions::CannotAccessTextureFile("Cannot load texture: " + path);
    }
}
}