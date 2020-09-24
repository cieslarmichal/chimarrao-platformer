#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "TexturePath.h"

namespace animations
{
using TextureChanged = bool;

class Animation
{
public:
    explicit Animation(std::vector<graphics::TexturePath> consecutiveTexturePaths, float timeBetweenTextures);

    TextureChanged update(const utils::DeltaTime&);
    void reset();
    [[nodiscard]] const graphics::TexturePath& getCurrentTexturePath() const;

private:
    void moveToNextTexture();

    std::vector<graphics::TexturePath> consecutiveTexturePaths;
    std::vector<graphics::TexturePath>::iterator currentTextureIter;
    float timeBetweenTextures;
    float timeUntilNextTexture;
};
}