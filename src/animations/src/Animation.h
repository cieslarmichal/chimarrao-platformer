#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "TextureRect.h"

namespace animations
{
using TextureRectChanged = bool;

class Animation
{
public:
    explicit Animation(std::vector<graphics::TextureRect> consecutiveTextureRects, float timeBetweenTextures);

    TextureRectChanged update(const utils::DeltaTime&);
    void reset();
    [[nodiscard]] const graphics::TextureRect& getCurrentTextureRect() const;

private:
    void moveToNextTextureRect();

    std::vector<graphics::TextureRect> consecutiveTextureRects;
    std::vector<graphics::TextureRect>::iterator currentTextureRectIter;
    float timeBetweenTextures;
    float timeUntilNextTexture;
};
}