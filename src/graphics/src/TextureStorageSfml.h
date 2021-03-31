#pragma once

#include <memory>
#include <unordered_map>

#include "IntRect.h"
#include "TextureLoader.h"
#include "TextureRect.h"
#include "TextureStorage.h"

namespace graphics
{
class TextureStorageSfml : public TextureStorage
{
public:
    const sf::Texture& getTexture(const TextureRect& textureRect) override;

private:
    void loadTexture(const TextureRect& path);
    bool textureInStorage(const TextureRect& path);

    std::unordered_map<TextureRect, std::unique_ptr<sf::Texture>, TextureRectHash> textures;
};
}