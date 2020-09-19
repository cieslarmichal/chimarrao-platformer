#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "TexturePath.h"

namespace graphics::animation
{
using TextureChanged = bool;

class Animation
{
public:
    explicit Animation(std::vector<TexturePath> consecutiveTexturePaths, float timeBetweenTextures);

    TextureChanged update(const utils::DeltaTime&);
    void reset();
    [[nodiscard]] const TexturePath& getCurrentTexturePath() const;

private:
    void moveToNextTexture();

    std::vector<TexturePath> consecutiveTexturePaths;
    std::vector<TexturePath>::iterator currentTextureIter;
    float timeBetweenTextures;
    float timeUntilNextTexture;
};
}