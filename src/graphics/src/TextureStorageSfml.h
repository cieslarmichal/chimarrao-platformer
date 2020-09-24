#pragma once

#include <memory>
#include <unordered_map>

#include "TextureLoader.h"
#include "TextureStorage.h"

namespace graphics
{
class TextureStorageSfml : public TextureStorage
{
public:
    const sf::Texture& getTexture(const TexturePath& path) override;

private:
    void loadTexture(const TexturePath& path);
    bool textureInStorage(const TexturePath& path);

    std::unordered_map<TexturePath, std::unique_ptr<sf::Texture>> textures;
};
}