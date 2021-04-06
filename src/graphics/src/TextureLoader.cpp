#include "TextureLoader.h"

#include "exceptions/CannotAccessTextureFile.h"

namespace graphics
{
void graphics::TextureLoader::load(sf::Texture& texture, const TextureRect& textureRect)
{
    if (textureRect.rectToCutTexture)
    {
        if (texture.loadFromFile(textureRect.texturePath, *textureRect.rectToCutTexture))
        {
            return;
        }
    }
    else
    {
        if (texture.loadFromFile(textureRect.texturePath))
        {
            return;
        }
    }

    throw exceptions::CannotAccessTextureFile("Cannot load texture rect: " + toString(textureRect));
}
}