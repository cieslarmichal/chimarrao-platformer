#pragma once

#include <string>
#include <vector>
#include "DeltaTime.h"
#include "TexturePath.h"

namespace graphics
{
class Animation
{
public:
    explicit Animation(std::vector<TexturePath> consecutiveTexturePaths, float timeBetweenTextures);

    void update(const utils::DeltaTime&);
    void reset();
    const TexturePath& getCurrentTexturePath() const;

private:
    void moveToNextTexture();

    std::vector<TexturePath> consecutiveTexturePaths;
    std::vector<TexturePath>::iterator currentTextureIter;
    float timeBetweenTextures;
    float timeUntilNextTexture;
};
}